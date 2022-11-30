/*
 * gamelogic.cc
 *
 *  Created on: 15.11.2022
 *      Author: blox
 */

#include "gamelogic.h"

#include <algorithm>
#include "arrakis.h"

std::vector<GameLogic::AllowedAction> GameLogic::msAllowedActions =
{
	{PHASE_INIT_PREDICTION,        true, Faction::beneGesserit(),                  ACTION_PREDICT},
	{PHASE_INIT_TRAITOR_SELECTION, true, Faction::anyExcept(Faction::harkonnen()), ACTION_TRAITOR_SELECTION},
	{PHASE_INIT_FREMEN_PLACEMENT,  true, Faction::fremen(),                        ACTION_FREMEN_PLACEMENT}
};

template<>
std::unordered_map<GameLogic::GamePhase, const char*> GameLogic::GamePhaseLabels::labels =
{
	{GameLogic::PHASE_INIT_PREDICTION,        "init.prediction"},
	{GameLogic::PHASE_INIT_TRAITOR_SELECTION, "init.traitorSelection"},
	{GameLogic::PHASE_INIT_FREMEN_PLACEMENT,  "init.fremenPlacement"}
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

void GameLogic::advance(GameState& game, GamePhase next)
{

	auto it = std::find_if(msAllowedActions.cbegin(), msAllowedActions.cend(),
			[next](const AllowedAction& a){ return a.mainAction && (a.when == next); });

	if (it != msAllowedActions.end())
		mGame.expectingInputFrom = it->who;

	mGame.phase = next;
	log->info("advance game -> %s | input required from %s", GamePhaseLabels::label(game.phase), mGame.expectingInputFrom.label().c_str());
}

void GameLogic::drawTraitors(GameState& game)
{
	auto& deck = game.traitors;
	for (auto& player : game.players)
	{
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
}

bool GameLogic::factionAvailable(GameState& game, Faction faction)
{
	for (const auto& player: game.players)
	{
		if (player.faction == faction)
			return true;
	}
	return false;
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



bool GameLogic::phaseInitPrediction(GameState& game, const Action& action)
{
	if (action.type() != ACTION_PREDICT)
		return false;

	const auto& prediction = static_cast<const ActionPrediction&> (action);

	game.predictedFaction = prediction.winner;
	game.predictedTurn = prediction.round;
	log->info("set prediction to %s in turn %d", game.predictedFaction.label().c_str(), game.predictedTurn);

	advance(game, PHASE_INIT_TRAITOR_SELECTION);
	return true;
}

bool GameLogic::phaseInitTraitorSelect(GameState& game, const Action& action)
{
	if (action.type() != ACTION_TRAITOR_SELECTION)
		return false;
	if (!expected(game, action.from()))
		return false;

	auto* state = getPlayerState(game, action.from());
	if (state == nullptr)
		return false;

	const auto& selection = static_cast<const ActionTraitorSelection&> (action);

	int found = -1;
	for (std::size_t i = 0; i < state->selectedTraitors.size(); ++i)
	{
		if (state->selectedTraitors[i] == selection.selection)
		{
			found = i;
			break;
		}
	}

	if (found == -1)
		return false;

	log->info("%s selected traitor %u", selection.from().label().c_str(), selection.selection);
	std::swap(state->selectedTraitors, state->discardedTraitors);
	state->selectedTraitors.push_back(state->discardedTraitors[found]);

	auto it = state->discardedTraitors.begin();
	std::advance(it, found);
	state->discardedTraitors.erase(it);

	game.expectingInputFrom.clear(action.from());

	if (game.expectingInputFrom == Faction::none())
	{
		if (factionAvailable(game, Faction::fremen()))
		{
			advance(game, PHASE_INIT_FREMEN_PLACEMENT);
		}
		else
		{
			game.phase = PHASE_INIT_end;
		}
	}

	return true;
}

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


	drawTraitors(mGame);

	if (factionAvailable(mGame, Faction::beneGesserit()))
		advance(mGame, PHASE_INIT_PREDICTION);
	else
		advance(mGame, PHASE_INIT_TRAITOR_SELECTION);
}

bool GameLogic::gameAction(GameState& game, const Action& action)
{
	if (!isAllowedAction(game, action))
		return false;

	switch (game.phase)
	{
	case PHASE_INIT_PREDICTION:
		return phaseInitPrediction(game, action);
	case PHASE_INIT_TRAITOR_SELECTION:
		return phaseInitTraitorSelect(game, action);
	default:
		return false;
	}
}

void GameLogic::record(std::unique_ptr<const Action>&& action)
{
	mRecorded.push_back(std::move(action));
}

GameLogic::TraitorDeck::TraitorDeck(Faction factionsInGame, std::mt19937& random)
: mpRandom(&random)
{
	for (int i = static_cast<int> (Leader::LEADERS_begin); i < Leader::LEADERS_end; ++i)
	{
		drawPile.push_back(static_cast<Leader::Id> (i));
	}

    std::shuffle(drawPile.begin(), drawPile.end(), random);
}

Leader::Id GameLogic::TraitorDeck::draw()
{
	if (drawPile.empty())
		reshuffle();

	drawn.push_back(peek());
	drawPile.pop_back();
	return drawn.back();
}

Leader::Id GameLogic::TraitorDeck::peek() const noexcept
{
	return drawPile.back();
}

void GameLogic::TraitorDeck::discard(Leader::Id card)
{
	for (auto it = drawn.begin(); it < drawn.end(); ++it)
	{
		if (*it == card)
		{
			discardPile.push_back(card);
			drawn.erase(it);
		}
	}
}

void GameLogic::TraitorDeck::reshuffle()
{
	drawPile.insert(drawPile.end(), discardPile.begin(), discardPile.end());
	discardPile.clear();

    std::shuffle(drawPile.begin(), drawPile.end(), *mpRandom);
}

GameLogic::PlayerState*
GameLogic::getPlayerState(GameState& game, Faction faction)
{
	auto it = std::find_if(game.players.begin(), game.players.end(),
				[faction] (const PlayerState& s) -> bool { return s.faction == faction; });
	if (it != game.players.end())
		return &(*it);
	else
		return nullptr;
}

GameLogic::PlayerState GameLogic::PlayerState::create(int aSeat, Faction aFaction)
{
	if (aFaction == Faction::emperor())
		return PlayerState(aSeat, Faction::emperor(),       10, 15, 5);
	if (aFaction == Faction::spacingGuild())
		return PlayerState(aSeat, Faction::spacingGuild(),  5,  20, 0);
	if (aFaction == Faction::atreides())
		return PlayerState(aSeat, Faction::atreides(),      10, 20, 0);
	if (aFaction == Faction::harkonnen())
		return PlayerState(aSeat, Faction::harkonnen(),     10, 20, 0);
	if (aFaction == Faction::fremen())
		return PlayerState(aSeat, Faction::fremen(),        3,  15, 5);
	if (aFaction == Faction::beneGesserit())
		return PlayerState(aSeat, Faction::beneGesserit(),  5,  20, 0);
	if (aFaction == Faction::tleilaxu())
		return PlayerState(aSeat, Faction::tleilaxu(),      5,  20, 0);
	else
		return PlayerState();
}


int main()
{
	GameLogic game(Faction::any(), 4004030159);
	game.post(std::make_unique<ActionPrediction>(Faction::beneGesserit(), Faction::atreides(), 5));

	game.post(std::make_unique<ActionTraitorSelection>(Faction::emperor(),      Leader::ID_Irulan));
	game.post(std::make_unique<ActionTraitorSelection>(Faction::spacingGuild(), Leader::ID_Burseg));
	game.post(std::make_unique<ActionTraitorSelection>(Faction::fremen(),       Leader::ID_Alia));
	game.post(std::make_unique<ActionTraitorSelection>(Faction::atreides(),     Leader::ID_Feyd));
	game.post(std::make_unique<ActionTraitorSelection>(Faction::beneGesserit(), Leader::ID_Duncan));
	game.post(std::make_unique<ActionTraitorSelection>(Faction::tleilaxu(),     Leader::ID_Wanna));

	game.tick();
	return 0;
}





