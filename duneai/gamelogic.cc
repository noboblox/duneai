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
	{PHASE_INIT_BG_PLACEMENT,      true, Faction::beneGesserit(),                  ACTION_BENE_GESSERIT_START_FORCE},
	{PHASE_STORM_INITAL_DIAL,      true, Faction::any(),                           ACTION_STORM_INITIAL_DIAL},
	{PHASE_CHOAM_CHARITY,          true, Faction::any(),                           ACTION_CHOAM_CHARITY}
};

GameLogic::GameLogic()
: log(new StdoutLogger())
{
}

GameLogic::GameLogic(Faction factionsInGame)
: log(new StdoutLogger())
{
	setup(factionsInGame);
}


GameLogic::GameLogic(Faction factionsInGame, unsigned aSeed)
: log(new StdoutLogger())
{
	setup(factionsInGame, aSeed);
}

void GameLogic::setup(Faction factionsInGame)
{
	std::random_device rd;
	setup(factionsInGame,  rd());
}

void GameLogic::setup(Faction factionsInGame, unsigned aSeed)
{
	if (!initialized)
	{
		Init(mGame, factionsInGame, aSeed);
		initialized = true;
	}
}

void GameLogic::tick()
{
	while (!mPending.empty())
	{
		auto& ev = *mPending.front();

		if (ev.isSystemEvent())
		{
			systemEvent(ev.get<SystemEvent>());
		}
		else if (ev.isAction())
		{
			auto& action = ev.get<Action>();
			if (initialized && gameAction(mGame, action))
			{
				log->debug("record event %s from %s", action.label(), action.from().label().c_str());
				record(std::unique_ptr<Action>(static_cast<Action*> (mPending.front().release())));
			}
			else
			{
				log->info("discard event %s from %s", action.label(), action.from().label().c_str());
			}
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

void GameLogic::requestGameState(GameStateReceiver receiver)
{
	if (receiver)
		mPending.push(std::make_unique<SystemRequestGameState>(receiver));
}

void GameLogic::requestSave(SaveGameReceiver receiver)
{
	if (receiver)
		mPending.push(std::make_unique<SystemRequestSaveGame>(receiver));
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
	case PHASE_STORM_INITAL_DIAL:
		return phaseStormInitialStormDial(game, action);
	case PHASE_CHOAM_CHARITY:
		return phaseChoamCharity(game, action);
	default:
		return false;
	}
}

bool GameLogic::phaseInitPrediction(GameState& game, const Action& action)
{
	auto prediction = expectedAction<ActionPrediction>(game, action, ACTION_PREDICT);
	if (!prediction) return false;

	if (!prediction->winner.exactlyOne())
		return false;

	if (prediction->round < 1 || prediction->round > game.maxRound)
		return false;

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
		log->info("harkonnen mulligan. discard and redraw new traitors");
		discardTraitors(game);
		game.traitors.reshuffle();
		drawTraitors(game);
	}

	advance(game, PHASE_INIT_TRAITOR_SELECTION);
	return true;
}

bool GameLogic::phaseInitTraitorSelect(GameState& game, const Action& action)
{
	auto selection = expectedAction<ActionTraitorSelection> (game, action, ACTION_TRAITOR_SELECTION);
	if (!selection) return false;

	auto* player = getPlayerState(game, action.from());
	if (!player) return false;

	bool found = false;
	for (std::size_t i = 0; i < player->selectedTraitors.size(); ++i)
	{
		if (player->selectedTraitors[i] == selection->selection)
		{
			found = true;
			break;
		}
	}

	if (!found)
		return false;

	log->info("%s selected traitor %s", selection->from().label().c_str(), Leader::name(selection->selection));
	player->selectedTraitors.clear();
	player->selectedTraitors.push_back(selection->selection);

	game.expectingInputFrom.clear(action.from());

	if (game.expectingInputFrom == Faction::none())
	{
		if (factionAvailable(game, Faction::fremen()))
		{
			advance(game, PHASE_INIT_FREMEN_PLACEMENT);
			return true;
		}

		placeStaticStartForces(game);

		if (factionAvailable(game, Faction::beneGesserit()))
			advance(game, PHASE_INIT_BG_PLACEMENT);
		else
			advance(game, PHASE_STORM_INITAL_DIAL, initialStormDialFactions(game));
	}

	return true;
}

bool GameLogic::phaseInitFremenPlacement(GameState& game, const Action& action)
{
	auto ac = expectedAction<ActionFremenPlacement>(game, action, ACTION_FREMEN_PLACEMENT);
	if (!ac) return false;

	auto player = getPlayerState(game, action.from());
	if (!player) return false;

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

	if (sum_specials > player->specialForcesReserve)
	{
		log->warn("sum of fedaykin tokens to place %u > %u", sum_specials, player->specialForcesReserve);
		return false;
	}

	for (const auto& p : ac->placements)
	{
		game.board.placeHostile(ac->from(), p);
		player->reserve -= p.normal;
		player->specialForcesReserve -= p.special;
		log->info("add %u normals and %u fedaykin to area %s", p.normal, p.special, Arrakis::areaName(p.where));
		log->info("fremen now has %u normals and %u fedaykin in reserve", player->reserve, player->specialForcesReserve);
	}

	placeStaticStartForces(game);

	if (factionAvailable(game, Faction::beneGesserit()))
		advance(game, PHASE_INIT_BG_PLACEMENT);
	else
		advance(game, PHASE_STORM_INITAL_DIAL, initialStormDialFactions(game));

	return true;
}

bool GameLogic::phaseInitBeneGesseritPlacement(GameState& game, const Action& action)
{
	auto ac = expectedAction<ActionBeneGesseritStartingForce>(game, action, ACTION_BENE_GESSERIT_START_FORCE);
	if (!ac) return false;

	auto player = getPlayerState(game, action.from());
	if (!player) return false;

	if (game.board.hostileFactionsInTerritory(ac->where) > 0)
	{
		game.board.placeNeutral(player->faction, Placement{ac->where, 1, 0});
		player->reserve -= 1;
		log->info("place beneGesserit force in %s as advisor", Arrakis::areaName(ac->where));
	}
	else
	{
		game.board.placeHostile(player->faction, Placement{ac->where, 1, 0});
		player->reserve -= 1;
		log->info("place beneGesserit force in %s as fighter", Arrakis::areaName(ac->where));
	}

	advance(game, PHASE_STORM_INITAL_DIAL, initialStormDialFactions(game));
	return true;
}

bool GameLogic::phaseStormInitialStormDial(GameState& game, const Action& action)
{
	auto ac = expectedAction<ActionStormInitialDial>(game, action, ACTION_STORM_INITIAL_DIAL);
	if (!ac) return false;

	if (ac->dial < 0 || ac->dial > 20)
		return false;

	if (game.initialStormDial[0].first == Faction::none())
		game.initialStormDial[0] = std::make_pair(action.from(), ac->dial);
	else
		game.initialStormDial[1] = std::make_pair(action.from(), ac->dial);

	game.expectingInputFrom.clear(action.from());

	if (game.expectingInputFrom != Faction::none())
		return true;

	const auto& dial = game.initialStormDial;
	log->info("initial storm dial: %d (%s) + %d (%s) = %d", dial[0].second, dial[0].first.label().c_str(),
			  dial[1].second, dial[1].first.label().c_str(), dial[0].second + dial[1].second);

	const int toAdvance = (dial[0].second + dial[1].second) % 18;
	log->info("initial storm advances %d sectors", toAdvance);
	const int storm = game.board.advanceStorm(toAdvance);
	log->info("storm at sector %d", storm);


	advance(game, PHASE_SPICE_SPICE_BLOW);
	phaseSpiceSpiceBlow(game);
	return true;
}

void GameLogic::phaseSpiceSpiceBlow(GameState& game)
{
	// TODO basic rule 1 blow
	// TODO round > 1

	int territoriesDrawn = 0;
	int shaiHuluds = 0;

	while (territoriesDrawn < 2)
	{
		const auto& card = game.spiceDeck.draw();
		if (card.id() == SpiceCard::SHAI_HULUD)
		{
			++shaiHuluds;
			game.spiceDeck.placeDrawBottom(card);
			log->info("shai hulud is ignored in round 1");
		}
		else
		{
			++territoriesDrawn;
			game.board.addSpice(card.area(), card.base());
			log->info("add %d spice to %s", card.base(), Arrakis::areaName(card.area()));

			if (territoriesDrawn == 1)
				game.spiceDeck.discardA(card);
			else
				game.spiceDeck.discardB(card);
		}
	}

	if (shaiHuluds > 0)
	{
		game.spiceDeck.reshuffle();
		log->info("drawn %d shai huluds -> reshuffle deck", shaiHuluds);
	}

	advance(game, PHASE_CHOAM_CHARITY);
}

bool GameLogic::phaseChoamCharity(GameState& game, const Action& action)
{
	auto ac = expectedAction<ActionChoamCharity>(game, action, ACTION_CHOAM_CHARITY);
	if (!ac) return false;

	auto player = getPlayerState(game, action.from());

	if (ac->need)
	{
		int amount = 2;

		if (action.from() != Faction::beneGesserit())
			amount -= player->spice;

		if (amount <= 0)
			return false;

		player->spice += amount;
		log->info("%s receives %d spice", action.from().label().c_str(), amount);
	}

	game.expectingInputFrom.clear(action.from());
	return true;
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

	mGame.board = Arrakis(seats);
	mGame.traitors = TraitorDeck(factionsInGame, mGame.random);
	drawTraitors(mGame);
	mGame.spiceDeck = SpiceDeck(mGame.random);
	mGame.treacheryDeck = TreacheryDeck(mGame.random);

	for (auto& p : mGame.players)
	{
		p.hand.push_back(mGame.treacheryDeck.draw());
		log->info("starting card %d for %s", p.hand[0], p.faction.label().c_str());

		if (p.faction == Faction::harkonnen())
		{
			p.hand.push_back(mGame.treacheryDeck.draw());
			log->info("starting card %d for %s", p.hand[1], p.faction.label().c_str());
		}
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

PlayerState*
GameLogic::getPlayerState(GameState& game, int seat)
{
	auto it = std::find_if(game.players.begin(), game.players.end(),
				[seat] (const PlayerState& s) -> bool { return s.seat == seat; });
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

void GameLogic::advance(GameState& game, GamePhase next, Faction customFactions)
{

	auto it = std::find_if(msAllowedActions.cbegin(), msAllowedActions.cend(),
			[next](const AllowedAction& a){ return a.mainAction && (a.when == next); });

	if (customFactions != Faction::none())
		mGame.expectingInputFrom = customFactions;
	else if (it != msAllowedActions.end())
		mGame.expectingInputFrom = it->who;
	else
		mGame.expectingInputFrom = Faction::none();

	mGame.phase = next;
	log->info("advance game -> %s | input required from %s", GamePhaseLabels::label(game.phase), mGame.expectingInputFrom.label().c_str());
}

void GameLogic::discardTraitors(GameState& game)
{
	for (auto& player : mGame.players)
	{
		auto& deck = game.traitors;

		for (const auto& traitor : player.selectedTraitors)
		{
			deck.discard(traitor);
		}

		player.selectedTraitors.clear();
	}

}

void GameLogic::drawTraitors(GameState& game)
{
	for (auto& player : mGame.players)
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
		log->info("traitors drawn for %-12s: { %-20s, %-20s, %-20s, %-20s }", player.faction.label().c_str(),
														  Leader::name(drawn[0]),
														  Leader::name(drawn[1]),
														  Leader::name(drawn[2]),
														  Leader::name(drawn[3]));
	}
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

void GameLogic::placeStaticStartForces(GameState& game)
{
	auto& board = game.board;
	PlayerState* player = nullptr;

	if ((player = getPlayerState(game, Faction::harkonnen())) != nullptr)
	{
		board.placeHostile(player->faction, Placement{Carthag, 10, 0});
		player->reserve -= 10;
	}
	if ((player = getPlayerState(game, Faction::atreides())) != nullptr)
	{
		board.placeHostile(player->faction, Placement{Arrakeen, 10, 0});
		player->reserve -= 10;
	}
	if ((player = getPlayerState(game, Faction::spacingGuild())) != nullptr)
	{
		board.placeHostile(player->faction, Placement{TueksSietch, 5, 0});
		player->reserve -= 5;
	}
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

Faction GameLogic::initialStormDialFactions(GameState& game)
{
	Faction result = Faction::none();

	result |= getPlayerState(game, game.board.firstByStormOrder())->faction;
	result |= getPlayerState(game, game.board.lastByStormOrder())->faction;
	return result;
}

void GameLogic::systemEvent(const SystemEvent& event)
{
	switch (event.type())
	{
	case SystemEvent::T_GAMESTATE_REQUEST:
		static_cast<const SystemRequestGameState&>(event).invokeTarget(mGame);
		break;
	case SystemEvent::T_SAVE_GAME:
		static_cast<const SystemRequestSaveGame&>(event).invokeTarget(SaveGame(mGame, mRecorded));
		break;
	default:
		break;
	}
}
