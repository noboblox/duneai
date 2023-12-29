#include "conflicts.h"
#include "conflict.h"

Conflicts::Conflicts()
: mConflicts(),
  mAttacker(Faction::none())
{
}

Conflicts::Conflicts(std::vector<ForcesInArea>&& contestedAreas, Faction attacker)
: mConflicts(createConflicts(contestedAreas, attacker)),
  mAttacker(attacker)
{
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

std::vector<Conflict> Conflicts::createConflicts(std::vector<ForcesInArea>& contestedAreas, Faction attacker)
{
	filterAttackerPresent(contestedAreas, attacker);
	filterConflicted(contestedAreas);
	return expand(contestedAreas, attacker);
}

void Conflicts::filterAttackerPresent(std::vector<ForcesInArea>& forces, Faction attacker)
{
	std::remove_if(forces.begin(), forces.end(), [&](const ForcesInArea& f) {
		return !f.hasHostileForces(attacker);
	});
}

void Conflicts::filterConflicted(std::vector<ForcesInArea>& forces)
{
	std::remove_if(forces.begin(), forces.end(), [](const ForcesInArea& f) {
		return f.countHostileFactions() < 2;
	});
}

std::vector<Conflict> Conflicts::expand(const std::vector<ForcesInArea>& filtered, Faction attacker)
{
	std::vector<Conflict> result;

	for (const auto& allForces : filtered)
	{
		allForces.forEach([&](const PlacedForces& f) {
			if (f.faction != attacker)
				result.emplace_back(allForces, attacker, f.faction);
		});
	}

	return result;
}

