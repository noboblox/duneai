#ifndef SHIPORMOVE_H_
#define SHIPORMOVE_H_

#include "gameconstants.h"

class GameState;

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
	bool move(AreaId from, AreaId to, int normalAmount, int specialAmount, bool useHajr = false);

	bool finished() const noexcept;

	Faction currentlyShipping() const noexcept;

	bool shipFromReserve(AreaId to, int normalAmount, int specialAmount);
	bool shipToReserve(AreaId from, int normalAmount, int specialAmount);
	bool shipCrossPlanet(AreaId from, AreaId to, int normalAmount, int specialAmount);

private:
	GameState* game;
};

#endif
