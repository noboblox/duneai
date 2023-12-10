#include "battle.h"

Battle::Battle() noexcept
: mConflict()
{
}

Battle::Battle(Conflict&& c)
: mConflict(std::move(c))
{
}
