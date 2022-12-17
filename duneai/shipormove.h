#ifndef SHIPORMOVE_H_
#define SHIPORMOVE_H_

#include "gameconstants.h"

class GameState;
class PlayerState;

class ShipOrMove
{
public:
	explicit ShipOrMove();
	explicit ShipOrMove(GameState& game);

	bool pendingGuildDecision() const noexcept;
	void forceGuildShipment() noexcept;
    void delayGuildShipment() noexcept;

	bool pendingIntrusionReaction() const noexcept;
	void disengageIntrusion() noexcept;
	void engageIntrusion() noexcept;

	bool pendingAccompanyDecision() const noexcept;
	void sendAdvisor(bool toPolarSink);
	void keepAdvisor();

	bool pendingMovement() const noexcept;
	bool passMovement();
	bool move(AreaId from, AreaId to, int normalAmount, int specialAmount, bool useHajr = false, bool advisor = false);

	bool finished() const noexcept;

	Faction currentlyShipping() const noexcept;

	bool shipFromReserve(AreaId to, int normalAmount, int specialAmount);
	bool shipToReserve(AreaId from, int normalAmount, int specialAmount);
	bool shipCrossPlanet(AreaId from, AreaId to, int normalAmount, int specialAmount);
	bool passShipment();

private:
	enum SubPhase
	{
		SP_GUILD_DECISION,
		SP_SHIP,
		SP_MOVE,
		SP_ACCOMPANY,
		SP_INTRUSION,
		SP_FINISHED,
	};

	enum GuildStatus
	{
		AVAILABLE,
		NOT_AVAILABLE,
		ACTIVE,
	};

	struct LastAction
	{
		enum Origin
		{
			SHIP_PHASE_TO_PLANET,
			SHIP_PHASE_SPECIAL,
			MOVE_PHASE,
			MOVE_PHASE_HAJR,
		};

		Faction who;
		AreaId  where;
		Origin origin;
	};

	void advanceAfterShip(LastAction&& aLast);
	void advanceAfterMove(LastAction&& aLast);
	void advanceToNextPlayer();
	bool verifyShipment(const PlayerState* player, int cost, AreaId* from, AreaId* to, int normalAmount, int specialAmount);
	bool verifyMovement(const PlayerState* player, AreaId from, AreaId to, int normalAmount, int specialAmount, bool advisor);
	bool verifyCommon(const PlayerState* player, AreaId* from, AreaId* to, int normalAmount, int specialAmount);
	void shipmentTransaction(PlayerState* player, int cost, AreaId* from, AreaId* to, int normalAmount, int specialAmount);
	void payShipment(PlayerState* player, int cost);
	int shipmentCost(Faction who, AreaId where, int amount) const;
	bool beneGesseritCanAccompany() const noexcept;
	bool beneGesseritCanReact() const noexcept;

private:
	GameState* game                = nullptr;
	SubPhase phase                 = SP_SHIP;
	std::vector<Faction> turnOrder = {};
	LastAction last                = LastAction{Faction::none(), AreaId::INVALID, LastAction::SHIP_PHASE_SPECIAL};
	int currentIndex               = 0;
	GuildStatus priorityShipping   = NOT_AVAILABLE;
	bool beneGesseritAvailable     = false;
};

#endif
