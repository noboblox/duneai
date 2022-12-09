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

    static void reachable(AreaId from, int storm, int movement, std::vector<AreaId>& result);
    static bool insideStorm(AreaId id, int storm);
    static bool fremenInitArea(AreaId id);
    static const char* areaName(AreaId id);

	Arrakis();
	explicit Arrakis(const std::vector<int>& aSeats);

	void placeHostile(Faction from, Placement source);
	void placeNeutral(Faction from, Placement source);

	int hostileFactionsInTerritory(AreaId childArea);
	int neutralFactionsInTerritory(AreaId childArea);

	int getStorm() const noexcept;

	int firstByStormOrder() const noexcept;
	int nextByStormOrder(int seatBefore) const noexcept;
	int prevByStormOrder(int seatAfter) const noexcept;
	int lastByStormOrder() const noexcept;

	int advanceStorm(int count);
	int addSpice(AreaId area, int amount);
	int removeSpice(AreaId area, int amount) noexcept;
	int getSpice(AreaId area) const noexcept;

	static const std::vector<Area>& allAreas() { return areas; }

private:
	void place(ForcesFrom&& source);
	std::vector<ForcesFrom*> collectFromSameTerritory(AreaId childArea, const Faction* filterFaction, const bool* filterHostile);

	std::vector<ForcesFrom> mForces;
	std::vector<std::pair<AreaId, int>> spice;
	std::vector<int> seats;
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
