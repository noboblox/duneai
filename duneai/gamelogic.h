#ifndef GAMELOGIC_H_
#define GAMELOGIC_H_

#include <queue>
#include <memory>
#include <vector>

#include "actions.h"
#include "gameconstants.h"
#include "logger.h"
#include "systemevents.h"

class GameLogic
{
public:

	/// create a game logic without executing the initial setup
	explicit GameLogic();

	/// create a game logic an perform the initial setup
	explicit GameLogic(Faction factionsInGame);
	explicit GameLogic(Faction factionsInGame, unsigned aSeed);

	/**
	 * setup a new game.
	 * @param factionsInGame
	 */
	void setup(Faction factionsInGame);
	void setup(Faction factionsInGame, unsigned aSeed);

	/**
	 * @brief update the game including evaluation of posted action
	 * @pre the game needs to be set-up via call to @ref setup
	 */
	void tick();

	/**
	 * @brief post an action to the game logic
	 * the action will be evaluated during the next tick
	 */
	void post(std::unique_ptr<Action>&& action);


	/**
	 * @brief request the logic to provide the game state to the @param receiver function
	 * The receiver will be called by the thread running the logic @ref tick
	 */
	using GameStateReceiver = std::function<void(const GameState&)>;
	void requestGameState(GameStateReceiver receiver);

	/**
	 * @brief request the logic to save the game
	 * The receiver will be called by the thread running the logic @ref tick
	 * The save game needs to be either processed or copied as it will be destroyed after the call returns
	 */
	using SaveGameReceiver = std::function<void(const SaveGame&)>;
	void requestSave(SaveGameReceiver receiver);

	/**
	 * @brief set a new logger for this logic instance.
	 * the default logger is @ref StdoutLogger
	 * @param aLogger new logger instance to use
	 */
	void setLogger(std::unique_ptr<const Logger>&& aLogger);

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
	bool phaseInitPrediction(GameState& game, const Action& action);
	bool phaseInitHarkonnenRedraw(GameState& game, const Action& action);
	bool phaseInitTraitorSelect(GameState& game, const Action& action);
	bool phaseInitFremenPlacement(GameState& game, const Action& action);
	bool phaseInitBeneGesseritPlacement(GameState& game, const Action& action);

	void systemEvent(const SystemEvent& event);
	bool isAllowedAction(GameState& game, const Action& action);
	bool expected(GameState& game, Faction faction);
	void advance(GameState& game, GamePhase next);
	void discardTraitors(GameState& game);
	void drawTraitors(GameState& game);
    bool factionAvailable(GameState& game, Faction faction);
    bool harkonnenMayRedraw(GameState& game);
    void placeStaticStartForces(GameState& game);
    void record(std::unique_ptr<const Action>&& action);
    PlayerState* getPlayerState(GameState& game, Faction faction);
    template <typename A> const A* expectedAction(GameState& game, const Action& action, ActionType type);

private:
    static std::vector<AllowedAction> msAllowedActions;

private:
    bool initialized = false;
    std::unique_ptr<const Logger> log;
    std::queue<std::unique_ptr<Event>> mPending;
    std::vector<std::unique_ptr<const Action>> mRecorded;
	GameState mGame;
};



#endif /* GAMELOGIC_H_ */
