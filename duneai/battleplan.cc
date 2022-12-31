#include "battleplan.h"
#include "statefunctions.h"

void BattlePlan::setOffense(TreacheryCard::Id card) noexcept
{
    data.offense = card;
    setKnown(KNOWN_OFFENSE, true);
}

void BattlePlan::clearOffense(bool unknown) noexcept
{
    data.offense = TreacheryCard::INVALID;
    setKnown(KNOWN_OFFENSE, !unknown);
}

bool BattlePlan::isOffenseKnown() const noexcept
{
    return known & KNOWN_OFFENSE;
}

TreacheryCard::Id BattlePlan::offence() const noexcept
{
    return data.offense;
}

void BattlePlan::setDefense(TreacheryCard::Id card) noexcept
{
    data.defense = card;
    setKnown(KNOWN_DEFENSE, true);
}

void BattlePlan::clearDefense(bool unknown) noexcept
{
    data.defense = TreacheryCard::INVALID;
    setKnown(KNOWN_DEFENSE, !unknown);
}

bool BattlePlan::isDefenseKnown() const noexcept
{
    return known & KNOWN_DEFENSE;
}

TreacheryCard::Id BattlePlan::defense() const noexcept
{
    return data.defense;
}

void BattlePlan::setLeader(Leader::Id leader) noexcept
{
    data.hero      = leader;
    data.cheapHero = TreacheryCard::INVALID;
    setKnown(KNOWN_HERO, true);
}

void BattlePlan::setCheapHeroLeader(TreacheryCard::Id leader) noexcept
{
    data.hero      = Leader::ID_INVALID;
    data.cheapHero = leader;
    setKnown(KNOWN_HERO, true);
}

void BattlePlan::clearLeader(bool unknown) noexcept
{
    data.hero      = Leader::ID_INVALID;
    data.cheapHero = TreacheryCard::INVALID;
    setKnown(KNOWN_HERO, true);
}

bool BattlePlan::isLeaderKnown() const noexcept
{
    return known & KNOWN_HERO;
}

std::pair<TreacheryCard::Id, Leader::Id> BattlePlan::leader() const noexcept
{
    return std::make_pair(data.cheapHero, data.hero);
}

void BattlePlan::setForceDial(int dial) noexcept
{
    data.forceDial = dial;
    setKnown(KNOWN_FORCE_DIAL, true);
}

void BattlePlan::clearForceDial(bool unknown) noexcept
{
    data.forceDial = 0;
    setKnown(KNOWN_FORCE_DIAL, !unknown);
}

bool BattlePlan::isForceDialKnown() const noexcept
{
    return known & KNOWN_FORCE_DIAL;
}

int BattlePlan::forceDial() const noexcept
{
    return data.forceDial;
}

void BattlePlan::setSpiceDial(int dial) noexcept
{
    data.spiceDial = dial;
    setKnown(KNOWN_SPICE_DIAL, true);
}

void BattlePlan::clearSpiceDial(bool unknown) noexcept
{
    data.spiceDial = 0;
    setKnown(KNOWN_SPICE_DIAL, !unknown);
}

bool BattlePlan::isSpiceDialKnown() const noexcept
{
    return known & KNOWN_SPICE_DIAL;
}

int BattlePlan::spiceDial() const noexcept
{
    return data.spiceDial;
}

static bool validateEmpty(const Player)
{
}

bool BattlePlan::validate(const PlayerState& player) const noexcept
{
    if (StateFunctions::aliveHeros(player) == 0 && !StateFunctions::ownsCheapHero(player))

}

void BattlePlan::setKnown(Known mask, bool value) noexcept
{
    if (value)
        known |= mask;
    else
        known &= (~mask);
}
