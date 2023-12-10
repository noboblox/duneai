#ifndef FACTIONPOSITION_H_
#define FACTIONPOSITION_H_

#include "gameconstants.h"

class FactionPosition
{
public:
	int seat;
	Faction faction;

	FactionPosition(int aSeat, Faction aFaction) : seat(aSeat), faction(aFaction) {}

	const bool operator!=(const FactionPosition& o) const noexcept { return !operator==(o); }
	const bool operator==(const FactionPosition& o) const noexcept { return seat == o.seat && faction == o.faction; }

	const bool operator>=(const FactionPosition& o) const noexcept { return !operator<(o); }
	const bool operator<(const FactionPosition& o) const noexcept
	{
		if (seat == o.seat)
			return faction < o.faction;
		return seat < o.seat;
	}

	const bool operator>(const FactionPosition& o) const noexcept { return o.operator<(*this); }
	const bool operator<=(const FactionPosition& o) const noexcept { return !(o.operator<(*this)); }
};

#endif /* FACTIONPOSITION_H_ */
