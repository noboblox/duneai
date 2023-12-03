#ifndef CONFLICTS_H_
#define CONFLICTS_H_

#include <algorithm>
#include <functional>
#include <vector>

#include "battle.h"
#include "forces.h"

class Arrakis;

class Conflicts
{
public:
	explicit Conflicts();
	explicit Conflicts(const Arrakis& board);

	bool empty() const noexcept;

	Faction aggressor() const noexcept;
	Faction competitors() const noexcept;

	bool needDecision() const noexcept;

	bool selectBattle(int id) noexcept;
	void clear() noexcept;
	Battle createNextBattle() const noexcept;

private:
};

#endif /* CONFLICTS_H_ */
