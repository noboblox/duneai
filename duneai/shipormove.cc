#include "shipormove.h"

ShipOrMove::ShipOrMove()
: game(nullptr)
{
}

ShipOrMove::ShipOrMove(GameState& aGame)
: game(&aGame)
{
}

bool ShipOrMove::pendingGuildDecision() const noexcept
{
}

void ShipOrMove::forceGuildShipment() noexcept
{
}

void ShipOrMove::delayGuildShipment() noexcept
{
}

bool ShipOrMove::pendingIntrusionReaction() const noexcept
{
}

void ShipOrMove::disengageIntrusion() noexcept
{
}

void ShipOrMove::engageIntrusion() noexcept
{
}

bool ShipOrMove::pendingAccompanyDecision() const noexcept
{
}

void ShipOrMove::sendAdvisor(bool toPolarSink)
{
}

void ShipOrMove::keepAdvisor()
{
}

bool ShipOrMove::pendingMovement() const noexcept
{
}

bool ShipOrMove::move(AreaId from, AreaId to, int normalAmount, int specialAmount, bool useHajr)
{
}

bool ShipOrMove::finished() const noexcept
{
}

Faction ShipOrMove::currentlyShipping() const noexcept
{
}

bool ShipOrMove::shipFromReserve(AreaId to, int normalAmount, int specialAmount)
{
}

bool ShipOrMove::shipToReserve(AreaId from, int normalAmount, int specialAmount)
{
}

bool ShipOrMove::shipCrossPlanet(AreaId from, AreaId to, int normalAmount, int specialAmount)
{
}
