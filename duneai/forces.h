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

class Conflict
{
public:
	explicit Conflict() : mParties(Faction::none()) {}

	bool isValid() const noexcept;
	void add(const ForcesFrom& participant);
	void removeStormArea(int storm);

	const std::vector<ForcesFrom>& forces() const noexcept { return mForces; }
	const Faction parties() const noexcept { return mParties; }

	static void partition(Conflict source, int storm, std::vector<Conflict>& addTo);

private:
	void updateParties() noexcept;

	Faction mParties;
	std::vector<ForcesFrom> mForces;
};


#endif



