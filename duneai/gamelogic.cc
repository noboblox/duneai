#include "gamelogic.h"

#include <algorithm>
#include <map>
#include "arrakis.h"
#include "gamedata.h"

std::vector<GameLogic::AllowedAction> GameLogic::msAllowedActions =
{
	{PHASE_INIT_PREDICTION,        true, Faction::beneGesserit(),                  ACTION_PREDICT},
	{PHASE_INIT_HARKONNEN_REDRAW,  true, Faction::harkonnen(),                     ACTION_HARKONNEN_REDRAW},
	{PHASE_INIT_TRAITOR_SELECTION, true, Faction::anyExcept(Faction::harkonnen()), ACTION_TRAITOR_SELECTION},
	{PHASE_INIT_FREMEN_PLACEMENT,  true, Faction::fremen(),                        ACTION_FREMEN_PLACEMENT},
	{PHASE_INIT_BG_PLACEMENT,      true, Faction::beneGesserit(),                  ACTION_BENE_GESSERIT_PLACEMENT}
};

GameLogic::GameLogic(Faction factionsInGame)
: log(new StdoutLogger())
{
	std::random_device rd;
	Init(mGame, factionsInGame, rd());
}

GameLogic::GameLogic(Faction factionsInGame, unsigned aSeed)
: log(new StdoutLogger())
{
	Init(mGame, factionsInGame, aSeed);
}

void GameLogic::tick()
{
	while (!mPending.empty())
	{
		auto& action = *mPending.front();
		if (gameAction(mGame, action))
		{
			log->debug("record event %s from %s", action.label(), action.from().label().c_str());
			record(std::move(mPending.front()));
		}
		else
		{
			log->info("discard event %s from %s", action.label(), action.from().label().c_str());
		}

		mPending.pop();
	}
}

void GameLogic::post(std::unique_ptr<Action>&& action)
{
	if (action != nullptr)
		mPending.push(std::move(action));
}

void GameLogic::setLogger(std::unique_ptr<const Logger>&& aLogger)
{
	log = std::move(aLogger);
}

//
//-- PHASES
//

bool GameLogic::gameAction(GameState& game, const Action& action)
{
	if (!isAllowedAction(game, action))
		return false;

	switch (game.phase)
	{
	case PHASE_INIT_PREDICTION:
		return phaseInitPrediction(game, action);
	case PHASE_INIT_HARKONNEN_REDRAW:
		return phaseInitHarkonnenRedraw(game, action);
	case PHASE_INIT_TRAITOR_SELECTION:
		return phaseInitTraitorSelect(game, action);
	case PHASE_INIT_FREMEN_PLACEMENT:
		return phaseInitFremenPlacement(game, action);
	case PHASE_INIT_BG_PLACEMENT:
		return phaseInitBeneGesseritPlacement(game, action);
	default:
		return false;
	}
}

bool GameLogic::phaseInitPrediction(GameState& game, const Action& action)
{
	auto prediction = expectedAction<ActionPrediction>(game, action, ACTION_PREDICT);
	if (!prediction) return false;

	game.predictedFaction = prediction->winner;
	game.predictedTurn = prediction->round;
	log->info("set prediction to %s in turn %d", game.predictedFaction.label().c_str(), game.predictedTurn);

	if (harkonnenMayRedraw(game))
		advance(game, PHASE_INIT_HARKONNEN_REDRAW);
	else
		advance(game, PHASE_INIT_TRAITOR_SELECTION);
	return true;
}

bool GameLogic::phaseInitHarkonnenRedraw(GameState& game, const Action& action)
{
	auto ac = expectedAction<ActionHarkonnenRedraw>(game, action, ACTION_HARKONNEN_REDRAW);
	if (!ac) return false;

	if (ac->redraw == true)
	{
		auto* player = getPlayerState(game, Faction::harkonnen());

		for (const auto& traitor : player->selectedTraitors)
		{
			game.traitors.discard(traitor);
		}

		player->selectedTraitors.clear();
		game.traitors.reshuffle();
		drawTraitors(game, *player);
	}

	if (harkonnenMayRedraw(game))
		advance(game, PHASE_INIT_HARKONNEN_REDRAW);
	else
		advance(game, PHASE_INIT_TRAITOR_SELECTION);

	return true;
}

bool GameLogic::phaseInitTraitorSelect(GameState& game, const Action& action)
{
	auto selection = expectedAction<ActionTraitorSelection> (game, action, ACTION_TRAITOR_SELECTION);
	if (!selection) return false;

	auto* state = getPlayerState(game, action.from());
	if (!state) return false;

	int found = -1;
	for (std::size_t i = 0; i < state->selectedTraitors.size(); ++i)
	{
		if (state->selectedTraitors[i] == selection->selection)
		{
			found = i;
			break;
		}
	}

	if (found == -1)
		return false;

	log->info("%s selected traitor %u", selection->from().label().c_str(), selection->selection);
	std::swap(state->selectedTraitors, state->discardedTraitors);
	state->selectedTraitors.push_back(state->discardedTraitors[found]);

	auto it = state->discardedTraitors.begin();
	std::advance(it, found);
	state->discardedTraitors.erase(it);

	game.expectingInputFrom.clear(action.from());

	if (game.expectingInputFrom == Faction::none())
	{
		if (factionAvailable(game, Faction::fremen()))
			advance(game, PHASE_INIT_FREMEN_PLACEMENT);
		else if (factionAvailable(game, Faction::beneGesserit()))
			advance(game, PHASE_INIT_BG_PLACEMENT);
		else
			advance(game, PHASE_INIT_end);
	}

	return true;
}

bool GameLogic::phaseInitFremenPlacement(GameState& game, const Action& action)
{
	auto ac = expectedAction<ActionFremenPlacement>(game, action, ACTION_FREMEN_PLACEMENT);
	if (!ac) return false;

	auto state = getPlayerState(game, action.from());
	if (!state) return false;

	int sum_normals = 0;
	int sum_specials = 0;

	for (const auto& p : ac->placements)
	{
		if (!Arrakis::fremenInitArea(p.where))
		{
			log->warn("area %s is not a fremen init area", Arrakis::areaName(p.where));
			return false;
		}

		sum_normals += p.normal;
		sum_specials += p.special;
	}

	if (sum_normals + sum_specials != 10)
	{
		log->warn("sum of tokens to place %u != 10", sum_normals + sum_specials);
		return false;
	}

	if (sum_specials > state->specialForcesReserve)
	{
		log->warn("sum of fedaykin tokens to place %u > %u", sum_specials, state->specialForcesReserve);
		return false;
	}

	std::map<Arrakis::AreaId, ForcesFrom> toPlace;

	for (const auto& p : ac->placements)
	{
		auto& value = toPlace[p.where];
		value.from = ac->from();
		value.forces.where = p.where;

		value.forces.normal += p.normal;
		value.forces.special += p.special;
	}

	for (const auto& p : toPlace)
	{
		game.forces.push_back(p.second);
		state->reserve -= p.second.forces.normal;
		state->specialForcesReserve -= p.second.forces.special;
		log->info("add %u normals and %u fedaykin to area %s", p.second.forces.normal, p.second.forces.special, Arrakis::areaName(p.second.forces.where));
		log->info("fremen now has %u normals and %u fedaykin in reserve", state->reserve, state->specialForcesReserve);
	}

	if (factionAvailable(game, Faction::beneGesserit()))
		advance(game, PHASE_INIT_BG_PLACEMENT);
	else
		advance(game, PHASE_INIT_end);

	return true;
}

bool GameLogic::phaseInitBeneGesseritPlacement(GameState& game, const Action& action)
{

}

//
//-- AUXILIARY
//

bool GameLogic::expected(GameState& game, Faction faction)
{
	return game.expectingInputFrom.contains(faction);
}

void GameLogic::Init(GameState& game, Faction factionsInGame, unsigned aSeed)
{
	const auto factions = Faction::expand(factionsInGame);

	if (factions.size() < 2)
	{
		log->crit("cannot instantiate games with less than 2 players");
		return;
	}
	if (factions.size() > 10)
	{
		log->crit("cannot instantiate games with more than 10 players");
		return;
	}

	mGame.seed = aSeed;
	mGame.random = std::mt19937(mGame.seed);
	log->info("set up game with seed %u", mGame.seed);

	std::vector<int> seats = SeatConfig::getConfig(factions.size());
	std::shuffle(seats.begin(), seats.end(), mGame.random);

	for (size_t i = 0; i < factions.size(); ++i)
	{
		mGame.players.push_back(PlayerState::create(seats[i], factions[i]));
		log->info("player %s at position %u", mGame.players.back().faction.label().c_str(), mGame.players.back().seat);
	}

	mGame.traitors = TraitorDeck(Faction::anyExcept(Faction::tleilaxu()), mGame.random);

	for (auto& player : mGame.players)
	{
		drawTraitors(mGame, player);
	}

	if (factionAvailable(mGame, Faction::beneGesserit()))
		advance(mGame, PHASE_INIT_PREDICTION);
	else if (harkonnenMayRedraw(mGame))
		advance(mGame, PHASE_INIT_HARKONNEN_REDRAW);
	else
		advance(mGame, PHASE_INIT_TRAITOR_SELECTION);
}

void GameLogic::record(std::unique_ptr<const Action>&& action)
{
	mRecorded.push_back(std::move(action));
}

PlayerState*
GameLogic::getPlayerState(GameState& game, Faction faction)
{
	auto it = std::find_if(game.players.begin(), game.players.end(),
				[faction] (const PlayerState& s) -> bool { return s.faction == faction; });
	if (it != game.players.end())
		return &(*it);
	else
		return nullptr;
}

template <typename A>
const A* GameLogic::expectedAction(GameState& game, const Action& action, ActionType type)
{
	if (action.type() != type)
		return nullptr;
	if (!expected(game, action.from()))
		return nullptr;
	return static_cast<const A*> (&action);
}

void GameLogic::advance(GameState& game, GamePhase next)
{

	auto it = std::find_if(msAllowedActions.cbegin(), msAllowedActions.cend(),
			[next](const AllowedAction& a){ return a.mainAction && (a.when == next); });

	if (it != msAllowedActions.end())
		mGame.expectingInputFrom = it->who;
	else
		mGame.expectingInputFrom = Faction::none();

	mGame.phase = next;
	log->info("advance game -> %s | input required from %s", GamePhaseLabels::label(game.phase), mGame.expectingInputFrom.label().c_str());
}

void GameLogic::drawTraitors(GameState& game, PlayerState& player)
{
	auto& deck = game.traitors;
	Leader::Id drawn[4] = {
		deck.draw(),
		deck.draw(),
		deck.draw(),
		deck.draw()
	};

	player.selectedTraitors.push_back(drawn[0]);
	player.selectedTraitors.push_back(drawn[1]);
	player.selectedTraitors.push_back(drawn[2]);
	player.selectedTraitors.push_back(drawn[3]);
	log->info("traitors drawn for %s: {%u,%u,%u,%u}", player.faction.label().c_str(),
													  drawn[0], drawn[1], drawn[2], drawn[3]);
}

bool GameLogic::factionAvailable(GameState& game, Faction faction)
{
	return getPlayerState(game, faction) != nullptr;
}

bool GameLogic::harkonnenMayRedraw(GameState& game)
{
	auto* state = getPlayerState(game, Faction::harkonnen());

	if (state == nullptr)
		return false;

	int own = 0;
	for (const auto id : state->selectedTraitors)
	{
		if (Leader::leader(id).faction() == Faction::harkonnen())
			++own;
	}

	return own > 1;
}

bool GameLogic::isAllowedAction(GameState& game, const Action& action)
{
	for (const auto& entry : msAllowedActions)
	{
		if (entry.when != game.phase)
			continue;
		if (!entry.who.contains(action.from()))
			continue;
		if (entry.what != action.type())
			continue;
		return true;
	}
	return false;
}
