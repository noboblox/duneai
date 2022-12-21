#ifndef ARRAKIS_H_
#define ARRAKIS_H_

#include <functional>
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
    static bool isSietch(AreaId id);
    static bool fremenInitArea(AreaId id);
    static bool fremenShipArea(AreaId id);
    static int areaSector(AreaId id);
    static int compareSector(int l, int r);
    static const char* areaName(AreaId id);

	Arrakis();
	explicit Arrakis(const std::vector<int>& aSeats, const std::vector<Faction>& aFactions);

	void place       (Faction from, Placement source, bool hostile);
	void placeHostile(Faction from, Placement source);
	void placeNeutral(Faction from, Placement source);
	void removeForces(Faction from, Placement source);

	bool setTerritoryHostility(Faction from, AreaId where, bool value);
	bool playerForcesInArea(Faction from, AreaId area, ForcesFrom& result) const;
	bool playerForcesInTerritory(Faction from, AreaId childArea, ForcesFrom& result) const;
	int hostileFactionsInTerritory(AreaId childArea) const;
	int hostileEnemiesInTerritory(Faction own, AreaId childArea) const;
	int neutralFactionsInTerritory(AreaId childArea) const;
	std::vector<Conflict> collectConflicts() const;

	int getStorm() const noexcept;

	bool canShip(Faction who, AreaId where) const;
	bool canMove(Faction who, AreaId from, AreaId to, bool moveAsHostiles = true);
	bool isOccupied(Faction shipper, AreaId where, bool moveAsHostiles = true) const;
	bool isReachable(AreaId from, AreaId to, int movement) const;
	bool isSameBattleground(AreaId areaA, AreaId areaB) const noexcept;
	int movementRange(Faction who) const noexcept;
	bool hasMovementBonus(Faction who) const noexcept;

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
	std::vector<const ForcesFrom*> collectFromSameTerritory(AreaId childArea, const Faction* filterFaction, const bool* filterHostile) const;
	std::vector<const ForcesFrom*> collectFromSameArea(AreaId childArea, const Faction* filterFaction, const bool* filterHostile) const;
	std::vector<const ForcesFrom*> collect(AreaId childArea, const Faction* filterFaction, const bool* filterHostile, bool restrictToArea) const;
	std::vector<Conflict> potentialConflicts() const;
	std::vector<Conflict> actualConflicts(const std::vector<Conflict>& potential, int storm) const;

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

	static void searchReachableAreas(AreaId from, int storm, int movement, const std::function<bool(AreaId)>& client);
	static bool sameTerritory(AreaId l, AreaId r) noexcept { return (l / 10) == (r / 10); }
    static void neighbors(AreaId from, int storm, const std::vector<AreaId>& exclude, std::vector<std::pair<AreaId, int>>& result);
    static const Area* getArea(AreaId id);
    static const Area* getTerritorySector(AreaId childArea, int sector);
};

#endif /* ARRAKIS_H_ */
