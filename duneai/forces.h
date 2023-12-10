#ifndef FORCES_H_
#define FORCES_H_

#include <array>
#include <functional>

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

	bool empty() const noexcept { return normal == 0 && special == 0;  }

	Faction faction = Faction::none();
	int normal      = 0;
	int special     = 0;
	bool hostile    = true;
};

class ForcesInArea
{
public:
	ForcesInArea() noexcept;
	ForcesInArea(const PartialTerritory& area);

	bool empty() const noexcept;
	bool hasNoHostileForces() const noexcept;

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

	void forEach(std::function<void(const PlacedForces&)> f) const;

	int countFactionIf(std::function<bool(const PlacedForces&)> f) const;
	int countHostileFactions(Faction except = Faction::none()) const;
	int countFactions(Faction except = Faction::none()) const;

private:
	PlacedForces& getForcesMutable(Faction who);
	std::array<PlacedForces, Faction::COUNT> placed;
	PartialTerritory mArea;
};

#endif



