#ifndef FORCES_H_
#define FORCES_H_

#include <array>

#include "gameconstants.h"
#include "partialterritory.h"

struct Placement
{
	AreaId where = AreaId::INVALID;
	int normal = 0;
	int special = 0;
};

struct PlacedForces
{
	PlacedForces(Faction aFaction, int aNormal, int aSpecial, bool aHostile = true)
	: faction(aFaction), normal(aNormal), special(aSpecial), hostile(aHostile)
	{
	}

	Faction faction = Faction::none();
	int normal      = 0;
	int special     = 0;
	bool hostile    = true;
};

class ForcesInArea
{
public:
	ForcesInArea();
	ForcesInArea(const PartialTerritory& area);

	const PlacedForces& getForces(Faction who) const;
	void setForces(const PlacedForces& value);
	void addForces(const PlacedForces& value);
	void removeForces(const PlacedForces& value);
	void setHostile(Faction who);
	void clearHostile(Faction who);

	int countHostileForces(Faction faction) const noexcept;
	bool hasHostileForces(Faction faction) const noexcept;

	int countNonHostileForces(Faction faction) const noexcept;
	bool hasNonHostileForces(Faction faction) const noexcept;

	int countForces(Faction faction) const noexcept;
	bool hasForces(Faction faction) const noexcept;

	const PartialTerritory& area() const noexcept;

	ForcesInArea& merge(const ForcesInArea& other);

	int countFactionIf(std::function<bool(const PlacedForces&)> f) const;

private:
	PlacedForces& getForcesMutable(Faction who);
	std::array<PlacedForces, Faction::COUNT> placed;
	PartialTerritory mArea;
};

#endif



