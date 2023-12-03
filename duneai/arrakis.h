#ifndef ARRAKIS_H_
#define ARRAKIS_H_

#include <functional>
#include <vector>

#include "forces.h"
#include "gameconstants.h"

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
    static bool isStronghold(AreaId id);
    static bool fremenInitArea(AreaId id);
    static bool fremenShipArea(AreaId id);
    static int areaSector(AreaId id);
    static int compareSector(int l, int r);
    static const char* areaName(AreaId id);

	Arrakis();
	explicit Arrakis(const std::vector<int>& aSeats, const std::vector<Faction>& aFactions);

	void place       (Faction from, Placement what, bool hostile);
	void placeHostile(Faction from, Placement what);
	void placeNeutral(Faction from, Placement what);
	void removeForces(Faction from, Placement what);

	bool setTerritoryHostility(Faction who, Territory where, bool value);
	ForcesInArea forcesInArea(const PartialTerritory& where) const;
	int hostileFactions(const PartialTerritory& where) const;
	int hostileEnemies(Faction own, const PartialTerritory& where) const;
	int neutralFactions(const PartialTerritory& where) const;

	int getStorm() const noexcept;
    bool insideStorm(AreaId id) const;

	bool canShip(Faction who, AreaId where) const;
	bool canMove(Faction who, AreaId from, AreaId to, bool moveAsHostiles = true);
	bool isOccupied(Faction who, AreaId where, bool moveAsHostiles = true) const;
	bool isReachable(AreaId from, AreaId to, int movement) const;
	int movementRange(Faction who) const noexcept;
	bool hasMovementBonus(Faction who) const noexcept;

	FactionPosition firstByStormOrder() const noexcept;
	FactionPosition lastByStormOrder() const noexcept;

	Faction firstOf(Faction factions) const noexcept;
	Faction lastOf(Faction factions) const noexcept;

	std::vector<FactionPosition> stormOrder() const;
	int advanceStorm(int count);
	int addSpice(AreaId area, int amount);
	int removeSpice(AreaId area, int amount) noexcept;
	int getSpice(AreaId area) const noexcept;

	static const std::vector<Area>& allAreas() { return areas; }

private:
	ForcesInArea& getOrCreate(AreaId area);

	void updateStormOrder();
	void place(AreaId where, const PlacedForces& what);

	ForcesInArea queryArea(PartialTerritory target) const;

	std::vector<ForcesInArea> mPlacements;

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
	static void neighbors(AreaId from, int storm, const std::vector<AreaId>& exclude, std::vector<std::pair<AreaId, int>>& result);
	static const Area* getArea(AreaId id);
    static const Area* getTerritorySector(AreaId childArea, int sector);
};

#endif /* ARRAKIS_H_ */
