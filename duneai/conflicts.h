#ifndef CONFLICTS_H_
#define CONFLICTS_H_

#include <climits>
#include <vector>

#include "conflict.h"
#include "forces.h"

class Conflicts
{
public:
	explicit Conflicts();
	explicit Conflicts(std::vector<ForcesInArea>&& contestedAreas, Faction attacker);

	bool empty() const noexcept;

	Faction attacker() const noexcept;

	bool needDecision() const noexcept;
	bool select(size_t index) noexcept;

	Conflict selected() const noexcept;
	const std::vector<Conflict>& list() const noexcept;

private:
	static std::vector<Conflict> createConflicts(std::vector<ForcesInArea>& contestedAreas, Faction attacker);

	static void filterAttackerPresent(std::vector<ForcesInArea>& forces, Faction attacker);
	static void filterConflicted(std::vector<ForcesInArea>& forces);
	static std::vector<Conflict> expand(const std::vector<ForcesInArea>& filtered, Faction attacker);

	std::vector<Conflict> mConflicts;
	Faction mAttacker;
	bool mIsSelected = false;
	size_t mSelectedIndex = 0;
};

#endif /* CONFLICTS_H_ */
