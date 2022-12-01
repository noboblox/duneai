#ifndef ARRAKIS_H_
#define ARRAKIS_H_

#include <vector>
#include "gameconstants.h"
#include "forces.h"

class Arrakis
{
public:
    static void reachable(AreaId from, int storm, int movement, std::vector<AreaId>& result);
    static bool insideStorm(AreaId id, int storm);
    static bool fremenInitArea(AreaId id);
    static const char* areaName(AreaId id);

	Arrakis();

	void placeHostile(Faction from, Placement source);
	void placeNeutral(Faction from, Placement source);

	int hostileFactionsInTerritory(AreaId childArea);
	int neutralFactionsInTerritory(AreaId childArea);

private:
	void place(ForcesFrom&& source);
	std::vector<ForcesFrom*> collectFromSameTerritory(AreaId childArea, const Faction* filterFaction, const bool* filterHostile);

	std::vector<ForcesFrom> mForces;

private:
	struct Area
	{
		const AreaId id;
		const char* name;
		const int sector;
	};

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
