#include "conflicts.h"

#include "arrakis.h"

Conflicts::Conflicts()
{
}

Conflicts::Conflicts(const Arrakis& board)
{
}

bool Conflicts::empty() const noexcept
{
	return false;
}


bool Conflicts::needDecision() const noexcept
{
	return false; // TODO
}

Faction Conflicts::aggressor() const noexcept
{
	return Faction::none(); // TODO
}

Faction Conflicts::competitors() const noexcept
{
	return Faction::none(); // TODO
}

bool Conflicts::selectBattle(int id) noexcept
{
	return false; // TODO
}

void Conflicts::clear() noexcept
{
}

Battle Conflicts::createNextBattle() const noexcept
{
	return Battle();
}

