#ifndef CONFLICT_H_
#define CONFLICT_H_

#include "forces.h"

class Conflict
{
public:
	Conflict() noexcept
	: mForces(), mAttacker(Faction::none()), mDefender(Faction::none())
	{
	}

	Conflict(const ForcesInArea& forces, Faction attacker, Faction defender) noexcept
	: mForces(forces), mAttacker(attacker), mDefender(defender)
	{
	}

	bool empty() const noexcept { return mAttacker == Faction::none() || mDefender == Faction::none(); }

	const ForcesInArea& forces() const noexcept { return mForces; }
	const Faction attacker() const noexcept { return mAttacker; }
	const Faction defender() const noexcept { return mDefender; }

private:
	ForcesInArea mForces;
	Faction mAttacker;
	Faction mDefender;
};

#endif /* CONFLICT_H_ */
