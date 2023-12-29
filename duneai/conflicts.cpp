#include "conflicts.h"
#include "conflict.h"

#include "factionposition.h"

Conflicts::Conflicts()
: mConflicts(),
  mAttacker(Faction::none())
{
}

Conflicts::Conflicts(std::vector<ForcesInArea>&& contestedAreas, const std::vector<FactionPosition>& stormOrder)
: mConflicts(),
  mAttacker(Faction::none())
{
	for (const auto& attacker: stormOrder)
	{
		mAttacker = attacker.faction;
		mConflicts = createConflicts(contestedAreas, mAttacker);

		if (!mConflicts.empty())
			return;
	}

	mAttacker = Faction::none();
}

bool Conflicts::empty() const noexcept
{
	return mConflicts.empty();
}


bool Conflicts::needDecision() const noexcept
{
	return !mIsSelected && mConflicts.size() > 1;
}

Faction Conflicts::attacker() const noexcept
{
	return mAttacker;
}

bool Conflicts::select(size_t index) noexcept
{
	if (index < 0 || index >= mConflicts.size())
		return false;

	mIsSelected = true;
	mSelectedIndex = index;
	return true;
}

Conflict Conflicts::selected() const noexcept
{
	if (mConflicts.empty())
		return Conflict();

	if (needDecision())
		return Conflict();

	return mConflicts[mSelectedIndex];
}

const std::vector<Conflict>& Conflicts::list() const noexcept
{
	return mConflicts;
}

std::vector<Conflict> Conflicts::createConflicts(const std::vector<ForcesInArea>& contestedAreas, Faction attacker)
{
	auto filteredAreas = contestedAreas;

	filterAttackerPresent(filteredAreas, attacker);
	filterConflicted(filteredAreas);
	return expand(filteredAreas, attacker);
}

void Conflicts::filterAttackerPresent(std::vector<ForcesInArea>& forces, Faction attacker)
{
	std::erase_if(forces, [&](const ForcesInArea& f) {
		return !f.hasHostileForces(attacker);
	});
}

void Conflicts::filterConflicted(std::vector<ForcesInArea>& forces)
{
	std::erase_if(forces, [](const ForcesInArea& f) {
		return f.countHostileFactions() < 2;
	});
}

std::vector<Conflict> Conflicts::expand(const std::vector<ForcesInArea>& filtered, Faction attacker)
{
	std::vector<Conflict> result;

	for (const auto& allForces : filtered)
	{
		allForces.forEach([&](const PlacedForces& f) {
			if (f.faction != attacker && f.hostile)
				result.emplace_back(allForces, attacker, f.faction);
		});
	}

	return result;
}

