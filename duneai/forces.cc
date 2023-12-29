#include "forces.h"

#include <algorithm>
#include <stdexcept>

#include "arrakis.h"

ForcesInArea::ForcesInArea() noexcept
: ForcesInArea(PartialTerritory())
{
}

ForcesInArea::ForcesInArea(const PartialTerritory& area)
: placed{
	PlacedForces(Faction::allFactions()[0], 0, 0, true),
	PlacedForces(Faction::allFactions()[1], 0, 0, true),
	PlacedForces(Faction::allFactions()[2], 0, 0, true),
	PlacedForces(Faction::allFactions()[3], 0, 0, true),
	PlacedForces(Faction::allFactions()[4], 0, 0, true),
	PlacedForces(Faction::allFactions()[5], 0, 0, true),
	PlacedForces(Faction::allFactions()[6], 0, 0, true),
  },
  mArea(area)
{
	  static_assert(Faction::COUNT == 7, "please update ForcesInArea::placed after changes to factions");
}

bool ForcesInArea::empty() const noexcept
{
	for (const auto& el: placed)
	{
		if (!el.empty())
			return false;
	}

	return true;
}

const PlacedForces& ForcesInArea::getForces(Faction who) const
{
	const auto& forces = const_cast<ForcesInArea&> (*this).getForcesMutable(who);
	return forces;
}

void ForcesInArea::setHostile(Faction who)
{
	auto& forces = getForcesMutable(who);
	forces.hostile = true;
}

void ForcesInArea::clearHostile(Faction who)
{
	auto& forces = getForcesMutable(who);
		forces.hostile = false;
}


PlacedForces& ForcesInArea::getForcesMutable(Faction who)
{
	auto it = std::find_if(placed.begin(), placed.end(), [&](const auto& el) {
		return el.faction == who;
	});

	if (it == placed.end())
		throw std::invalid_argument("faction is ambiguous");

	return *it;
}

void ForcesInArea::setForces(const PlacedForces& value)
{
	auto& f = getForcesMutable(value.faction);
	f = value;
}

void ForcesInArea::addForces(const PlacedForces& value)
{
	auto& f = getForcesMutable(value.faction);

	if (f.empty())
		f.hostile = value.hostile;

	f.normal += value.normal;
	f.special += value.special;
}

void ForcesInArea::removeForces(const PlacedForces& value)
{
	auto& f = getForcesMutable(value.faction);

	const int sub_normal = std::min(f.normal, value.normal);
	const int sub_special = std::min(f.special, value.special);

	f.normal -= sub_normal;
	f.special -= sub_special;
}



int ForcesInArea::countForces(Faction faction) const noexcept
{
	auto& f = getForces(faction);
	return f.normal + f.special;
}

bool ForcesInArea::hasForces(Faction faction) const noexcept
{
	return countForces(faction) != 0;
}

int ForcesInArea::countHostileForces(Faction faction) const noexcept
{
	auto& f = getForces(faction);
	return f.hostile ? f.normal + f.special : 0;
}

bool ForcesInArea::hasHostileForces(Faction faction) const noexcept
{
	return countHostileForces(faction) != 0;
}

int ForcesInArea::countNonHostileForces(Faction faction) const noexcept
{
	auto& f = getForces(faction);
	return f.hostile ? 0 : f.normal + f.special;
}

bool ForcesInArea::hasNonHostileForces(Faction faction) const noexcept
{
	return countNonHostileForces(faction) != 0;
}

const PartialTerritory& ForcesInArea::area() const noexcept
{
	return mArea;
}

ForcesInArea& ForcesInArea::merge(const ForcesInArea& other)
{
	for (int i = 0; i < Faction::COUNT; ++i)
	{
		placed[i].normal  += other.placed[i].normal;
		placed[i].special += other.placed[i].special;
		placed[i].hostile = other.placed[i].hostile;
	}

	mArea.merge(other.mArea);
	return *this;
}

int ForcesInArea::countHostileFactions(Faction except) const
{
	return countFactionIf([&](const PlacedForces& f){
		return f.hostile && !except.contains(f.faction);
	});
}

int ForcesInArea::countFactions(Faction except) const
{
	return countFactionIf([&](const PlacedForces& f){
			return !except.contains(f.faction);
	});
}

int ForcesInArea::countFactionIf(std::function<bool(const PlacedForces&)> f) const
{
	int result = 0;

	forEach([&](const PlacedForces& el) {

		if (f(el))
			++result;
	});

	return result;
}

void ForcesInArea::forEach(std::function<void(const PlacedForces&)> f) const
{
	for (const auto& el: placed)
	{
		if (el.empty())
			continue;

		f(el);
	}
}
