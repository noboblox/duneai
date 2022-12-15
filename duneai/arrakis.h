#ifndef ARRAKIS_H_
#define ARRAKIS_H_

#include <vector>
#include "gameconstants.h"
#include "forces.h"

class Arrakis
{
public:
	struct Area
	{
		const AreaId id;
		const char* name;
		const int sector;
	};

	struct FactionPosition
	{
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

    static void reachable(AreaId from, int storm, int movement, std::vector<AreaId>& result);
    static bool insideStorm(AreaId id, int storm);
    static bool fremenInitArea(AreaId id);
    static const char* areaName(AreaId id);

	Arrakis();
	explicit Arrakis(const std::vector<int>& aSeats, const std::vector<Faction>& aFactions);

	void placeHostile(Faction from, Placement source);
	void placeNeutral(Faction from, Placement source);

	int hostileFactionsInTerritory(AreaId childArea);
	int neutralFactionsInTerritory(AreaId childArea);

	int getStorm() const noexcept;

	FactionPosition firstByStormOrder() const noexcept;
	FactionPosition lastByStormOrder() const noexcept;
	std::vector<FactionPosition> stormOrder() const;
	int advanceStorm(int count);
	int addSpice(AreaId area, int amount);
	int removeSpice(AreaId area, int amount) noexcept;
	int getSpice(AreaId area) const noexcept;

	static const std::vector<Area>& allAreas() { return areas; }

private:
	void updateStormOrder();
	void place(ForcesFrom&& source);
	std::vector<ForcesFrom*> collectFromSameTerritory(AreaId childArea, const Faction* filterFaction, const bool* filterHostile);

	std::vector<ForcesFrom> mForces;
	std::vector<std::pair<AreaId, int>> spice;
	std::vector<FactionPosition> mStormOrder;
	int storm = 1;


private:
	struct Connection
	{
		const AreaId from;
		const AreaId to;
	};

	static const std::vector<Area> areas;
	static const std::vector<Connection> connections;

	static bool sameTerritory(AreaId l, AreaId r) noexcept { return (l / 10) == (r / 10); }
    static void neighbors(AreaId from, int storm, const std::vector<AreaId>& exclude, std::vector<std::pair<AreaId, int>>& result);
    static const Area* getArea(AreaId id);
};

#endif /* ARRAKIS_H_ */
