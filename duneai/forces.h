#ifndef FORCES_H_
#define FORCES_H_

#include "gameconstants.h"

struct Placement
{
	AreaId where = AreaId::INVALID;
	int normal = 0;
	int special = 0;
};

struct ForcesFrom : public Placement
{
	explicit ForcesFrom()
	: Placement{AreaId::INVALID, 0, 0}
	{
	}

	ForcesFrom(Faction aFrom, const Placement& aPlacement, bool aHostile)
	: Placement{aPlacement.where, aPlacement.normal, aPlacement.special}, from(aFrom), hostile(aHostile)
	{
	}



	Faction from = Faction::none();
	bool hostile = true;
};

#endif



