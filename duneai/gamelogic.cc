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

GameLogic::GameLogic()
: log(new StdoutLogger())
{
	std::random_device rd;
	Init(mGame, rd());
}

GameLogic::GameLogic(unsigned aSeed)
: log(new StdoutLogger())
{
	Init(mGame, aSeed);
}

#include <cstdio>

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
		Leader drawn[4] = {
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

void GameLogic::Init(GameState& game, unsigned aSeed)
{
	// Phase 0: INIT
	mGame.players.push_back(PlayerState(SEAT_A, Faction::spacingGuild(),  10, 20, 0));
	mGame.players.push_back(PlayerState(SEAT_B, Faction::emperor(),       10, 15, 5));
	mGame.players.push_back(PlayerState(SEAT_C, Faction::atreides(),      10, 20, 0));
	mGame.players.push_back(PlayerState(SEAT_D, Faction::harkonnen(),     10, 20, 0));
	mGame.players.push_back(PlayerState(SEAT_E, Faction::fremen(),        10, 15, 5));
	mGame.players.push_back(PlayerState(SEAT_F, Faction::beneGesserit(),  10, 20, 0));

	mGame.seed = aSeed;
	mGame.random = std::mt19937(mGame.seed);
	mGame.traitors = TraitorDeck(mGame.random);

	log->info("set up game with seed %u", mGame.seed);

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

GameLogic::TraitorDeck::TraitorDeck(std::mt19937& random)
: mpRandom(&random)
{
	for (int i = static_cast<int> (LEADERS_begin); i < LEADERS_end; ++i)
	{
		drawPile.push_back(static_cast<Leader> (i));
	}

    std::shuffle(drawPile.begin(), drawPile.end(), random);
}

Leader GameLogic::TraitorDeck::draw()
{
	if (drawPile.empty())
		reshuffle();

	drawn.push_back(peek());
	drawPile.pop_back();
	return drawn.back();
}

Leader GameLogic::TraitorDeck::peek() const noexcept
{
	return drawPile.back();
}

void GameLogic::TraitorDeck::discard(Leader card)
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

int main()
{
	GameLogic game(4004030159);
	game.post(std::make_unique<ActionPrediction>(Faction::beneGesserit(), Faction::atreides(), 5));
	game.post(std::make_unique<ActionTraitorSelection>(Faction::beneGesserit(), Leader::Piter));
	game.post(std::make_unique<ActionTraitorSelection>(Faction::atreides(), Leader::Esmar));
	game.post(std::make_unique<ActionTraitorSelection>(Faction::spacingGuild(), Leader::Stilgar));
	game.post(std::make_unique<ActionTraitorSelection>(Faction::emperor(), Leader::Alia));
	game.post(std::make_unique<ActionTraitorSelection>(Faction::fremen(), Leader::Yueh));

	game.tick();
	return 0;
}





