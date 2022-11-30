/*
 * gamelogic.h
 *
 *  Created on: 21.11.2022
 *      Author: Rene
 */

#ifndef GAMELOGIC_H_
#define GAMELOGIC_H_

#include <queue>
#include <memory>
#include <random>
#include <vector>

#include "actions.h"
#include "gameconstants.h"
#include "logger.h"

class GameLogic
{
public:
	explicit GameLogic(Faction factionsInGame);
	explicit GameLogic(Faction factionsInGame, unsigned aSeed);

	/**
	 * @brief update the game including evaluation of posted action
	 */
	void tick();

	/**
	 * @brief post an action to the game logic
	 * the action will be evaluated during the next tick
	 */
	void post(std::unique_ptr<Action>&& action);

	/**
	 * @brief set a new logger for this logic instance.
	 * the default logger is @ref StdoutLogger
	 * @param aLogger new logger instance to use
	 */
	void setLogger(std::unique_ptr<const Logger>&& aLogger);

	/// enum containing all stages of the game
	enum GamePhase
	{
		PHASE_invalid    = 0,

		PHASE_INIT_begin = 1,
		PHASE_INIT_PREDICTION = PHASE_INIT_begin,
		PHASE_INIT_TRAITOR_SELECTION,
		PHASE_INIT_FREMEN_PLACEMENT,
		PHASE_INIT_end    = 1000,

		PHASE_STORM_begin = PHASE_INIT_end,
		PHASE_STORM_end   = 2000,

		PHASE_SPICE_begin = PHASE_STORM_end,
		PHASE_SPICE_end   = 3000,
	};
	using GamePhaseLabels = EnumLabels<GamePhase>;

private:

	class TraitorDeck
	{
	public:
		explicit TraitorDeck() : mpRandom(nullptr) {}
		explicit TraitorDeck(Faction factionsInGame, std::mt19937& random);

		Leader::Id draw();
		Leader::Id peek() const noexcept;
		void discard(Leader::Id card);
		void reshuffle();

	private:
		std::mt19937* mpRandom;
		std::vector<Leader::Id> drawPile;
		std::vector<Leader::Id> discardPile;
		std::vector<Leader::Id> drawn;
	};

	struct PlayerState
	{
		static PlayerState create(int aSeat, Faction aFaction);

		PlayerState()
		: seat(0), faction(Faction::none()), spice(0), reserve(0), specialForcesReserve(0)
		{
		}

		PlayerState(int aSeat, Faction aFaction, int aSpice, int aReserve, int aSpecialForces)
		: seat(aSeat), faction(aFaction), spice(aSpice), reserve(aReserve), specialForcesReserve(aSpecialForces)
		{
		}

		int seat;
		Faction faction;

		int spice;
		int reserve;
		int specialForcesReserve;

		std::vector<Leader::Id> alive;
		std::vector<Leader::Id> selectedTraitors;
		std::vector<Leader::Id> discardedTraitors;
		std::vector<TreacheryCard> hand;

	};

	struct GameState
	{
		int round = 1;
		unsigned int seed = 0;
		std::mt19937 random;

		GamePhase phase = PHASE_invalid;
		Faction expectingInputFrom = Faction::none();

		int predictedTurn = 0;
		Faction predictedFaction = Faction::none();

		TraitorDeck traitors;
		std::vector<PlayerState> players;
	};

	struct AllowedAction
	{
		GamePhase    when;
		bool         mainAction;
		Faction      who;
		ActionType   what;
	};

private:
	void Init(GameState& game, Faction factionsInGame, unsigned aSeed);

	bool gameAction(GameState& game, const Action& action);

	bool isAllowedAction(GameState& game, const Action& action);
	bool expected(GameState& game, Faction faction);
	bool phaseInitPrediction(GameState& game, const Action& action);
	bool phaseInitTraitorSelect(GameState& game, const Action& action);

	void advance(GameState& game, GamePhase next);
	void drawTraitors(GameState& game);
    bool factionAvailable(GameState& game, Faction faction);
    void record(std::unique_ptr<const Action>&& action);
    PlayerState* getPlayerState(GameState& game, Faction faction);

private:
    static std::vector<AllowedAction> msAllowedActions;

private:
    std::unique_ptr<const Logger> log;
    std::queue<std::unique_ptr<Action>> mPending;
    std::vector<std::unique_ptr<const Action>> mRecorded;
	GameState mGame;
};



#endif /* GAMELOGIC_H_ */
