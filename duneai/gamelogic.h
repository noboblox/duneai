#ifndef GAMELOGIC_H_
#define GAMELOGIC_H_

#include <queue>
#include <memory>
#include <vector>

#include "actions.h"
#include "gameconstants.h"
#include "logger.h"
#include "deletedefaults.h"

class GameLogic
{
public:
	MACRO_DELETE_ALL_DEFAULTS(GameLogic)

	/**
	 * create a game logic without executing the initial setup
	 * @post at least two factions should be added via @ref addFaction
	 */
	explicit GameLogic(bool developerActions = false);

	/**
	 * add a faction to the game.
	 * @param faction faction to add
	 * @param gameMaster the faction may send game master actions
	 *
	 * @retval true: faction added
	 * @retval false: faction not added. This happens when the faction was already registered.
	 *
	 * @note can only be done before the game is set up
	 * @post post an action @ref GMActionSetupGame or call @ref setup
	 */
	bool addFaction(Faction faction, bool gameMaster = false) noexcept;

	bool idle() noexcept;

	/**
	 * remove a faction from the game.
	 * @param faction faction to remove
	 *
	 * @note can only be done before the game is set up
	 */
	void removeFaction(Faction faction) noexcept;

	/// setup the game with a random seed
	void setup();

	/// setup the game with a specified seed
	void setup(unsigned seed);

	/**
	 * @brief update the game including evaluation of posted action
	 * @pre the game needs to be either be setup via call to @ref setup
	 * or setup via a @ref post of the @ref GMActionSetupGame action
	 */
	void tick();

	/**
	 * Like @see tick, but execute only one event
	 */
	bool executeOne();

	/**
	 * @brief post an action to the game logic
	 * the action will be evaluated during the next tick
	 * @post call @ref tick to execute the posted actions
	 */
	void post(std::unique_ptr<Action>&& action);


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

	/**
	 * @brief some phases are processed automatically by the game logic and don't require input.
	 *
	 * Nonetheless, these phases can be triggered directly by a dev event.
	 *
	 * So the corresponding logic function needs to run when the game advances into this phase,
	 * whether it was reached internally or triggered by a dev event for testing purposes.
	 */
	struct PhaseExecutionFunction
	{
		GamePhase phase;
		std::function<void(GameLogic&, GameState&)> function;
	};

private:
	void setup(unsigned seed, StartActions initTasks);
	void init(GameState& game, Faction factionsInGame, unsigned aSeed, StartActions initTasks);
	void initCards(GameState& game);

	bool executeAction(GameState& game, const Action& action);
	bool gameMasterAction(GameState& game, const Action& action);
	bool devAction(GameState& game, const Action& action);
	bool userAction(GameState& game, const Action& action);

	// active phases
	bool phaseInitJoin(const Action& action);
	bool phaseInitPrediction(GameState& game, const Action& action);
	bool phaseInitHarkonnenRedraw(GameState& game, const Action& action);
	bool phaseInitTraitorSelect(GameState& game, const Action& action);
	bool phaseInitFremenPlacement(GameState& game, const Action& action);
	bool phaseInitBeneGesseritPlacement(GameState& game, const Action& action);
	bool phaseStormInitialStormDial(GameState& game, const Action& action);
	bool phaseChoamCharity(GameState& game, const Action& action);
	bool phaseBidding(GameState& game, const Action& action);
	bool phaseShipmentGuildDecision(GameState& game, const Action& action);
	bool phaseShipmentShip(GameState& game, const Action& action);
	bool phaseShipmentIntrusionReaction(GameState& game, const Action& action);
	bool phaseShipmentAccompanyDecision(GameState& game, const Action& action);
	bool phaseShipmentMove(GameState& game, const Action& action);
	bool phaseBattleSelection(GameState& game, const Action& action);

	// passive phases
	void phaseSpiceSpiceBlow(GameState& game);
	void phaseBattleCollectConflicts(GameState& game);
	void phaseBattleBegin(GameState& game);

	bool isAllowedAction(GameState& game, const Action& action);
	bool expected(GameState& game, Faction faction);
	int prepareAuction(GameState& game);
	void cleanupAuctionPool(GameState& game);
	void auctionWinTransaction(GameState& game, Faction won, int spice, bool karama);
	void advance(GameState& game, GamePhase next, Faction customFactions = Faction::none());
	void executeAutomaticTasks(GameState& game, GamePhase phase);
	void advanceToShipmentPhase(GameState& game);
	void advanceInShipmentPhase(GameState& game);
	void discardTraitors(GameState& game);
	void drawTraitors(GameState& game);
    bool factionAvailable(GameState& game, Faction faction);
    bool harkonnenMayRedraw(GameState& game);
    void placeStaticStartForces(GameState& game);
    std::vector<TreacheryCard>::iterator findKarama(GameState& game, Faction faction);
    bool hasKarama(GameState& game, Faction faction);
    void record(std::unique_ptr<const Action>&& action);
    Faction initialStormDialFactions(GameState& game);
    template <typename A> const A* expectedAction(GameState& game, const Action& action, ActionType type);


    bool devActionSetupCustom(GameState& game, const Action& action);
    bool devActionSetStorm(GameState& game, const Action& action);
    bool devActionSetPhase(GameState& game, const Action& action);
    bool devActionPlaceTroops(GameState& game, const Action& action);

private:
    static std::vector<AllowedAction> msAllowedActions;
    static std::vector<PhaseExecutionFunction> initPhaseFunctions(GameLogic& self);

private:
	bool mUseDevActions;
    bool initialized = false;
    const std::vector<PhaseExecutionFunction> mPhaseFunc;
    std::unique_ptr<const Logger> log;
    std::queue<std::unique_ptr<Action>> mPending;
    std::vector<std::unique_ptr<const Action>> mRecorded;
    Faction inGame = Faction::none();
    Faction gameMasters = Faction::none();
	GameState mGame;
};



#endif /* GAMELOGIC_H_ */
