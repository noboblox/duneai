#include "arrakis.h"

#include <algorithm>
#include <iterator>
#include "logger.h"

#include "territories.h"
#include "conflicts.h"
#include "stormposition.h"

Arrakis::Arrakis()
{
}

Arrakis::Arrakis(const std::vector<int>& aSeats, const std::vector<Faction>& aFactions)
: mStormOrder()
{
	mStormOrder.reserve(aSeats.size());
	for (std::size_t i = 0; i < aSeats.size(); ++i)
	{
		mStormOrder.emplace_back(FactionPosition{aSeats[i], aFactions[i]});
	}

	std::sort(mStormOrder.begin(), mStormOrder.end());
	updateStormOrder();
}

bool Arrakis::isReachable(AreaId from, AreaId to, int movement) const
{
	bool reachable = false;
	searchReachableAreas(from, storm, movement, [&reachable, &to](AreaId a)
			{
				if (a != to)
					return true;
				reachable = true;
				return false;
			});
	return reachable;
}

void Arrakis::reachable(AreaId from, int storm, int movement, std::vector<AreaId>& result)
{
	result.clear();
	searchReachableAreas(from,storm, movement, [&result](AreaId a)
	    {
			if (std::find(result.cbegin(), result.cend(), a) == result.cend())
				result.push_back(a);
			return true;
		});

}

void Arrakis::searchReachableAreas(AreaId from, int storm, int movement, const std::function<bool(AreaId)>& client)
{
	if (insideStorm(from, storm))
		return;

	std::vector<AreaId> visited;
	std::vector<std::pair<AreaId, int>> current, next;
	visited.reserve(20);
	current.reserve(10);
	next.reserve(10);

	current.emplace_back(from, 0);
	visited.push_back(from);

	while (!current.empty())
	{
		const auto from = current.front().first;
		neighbors(from, storm, visited, next);

		for (const auto& dest : next)
		{
			const int totalCost = current.front().second + dest.second;

			if (totalCost <= movement)
			{
				current.emplace_back(dest.first, totalCost);

				if (!client(dest.first))
					return;
			}
		}

		visited.push_back(from);
		current.erase(current.begin());
	}
}

void Arrakis::neighbors(AreaId from, int storm, const std::vector<AreaId>& exclude, std::vector<std::pair<AreaId, int>>& result)
{
	result.clear();

	auto it = std::find_if(connections.cbegin(), connections.cend(),
			[from](const Connection& c) -> bool { return c.from == from; });

	for (; it != connections.cend() && it->from == from; ++it)
	{
		if (insideStorm(it->to, storm))
			continue;
		if (std::find(exclude.cbegin(), exclude.cend(), it->to) != exclude.cend())
			continue;

		if (sameTerritory(it->from, it->to))
			result.emplace_back(it->to, 0);
		else
			result.emplace_back(it->to, 1);
	}
}

bool Arrakis::insideStorm(AreaId area, int storm)
{
	if (storm == 0)
		return false;

	return Territories::sectorOf(area) == storm;
}

bool Arrakis::isStronghold(AreaId area)
{
	switch (area)
	{
	case SietchTabr:
	case Arrakeen:
	case Carthag:
	case HabbanyaSietch:
	case TueksSietch:
		return true;
	default:
		return false;
	}
}

bool Arrakis::fremenInitArea(AreaId area)
{
	switch (area)
	{
	case SietchTabr:
	case FalseWallSouth_4:
	case FalseWallSouth_5:
	case FalseWallWest_16:
	case FalseWallWest_17:
	case FalseWallWest_18:
		return true;
	default:
		return false;
	}
}

static std::vector<AreaId> initFremenShipArea()
{
	static constexpr int STORM_OUTSIDE_FREMEN_AREA = 2;
	std::vector<AreaId> allowed;

	Arrakis::reachable(TheGreatFlat, STORM_OUTSIDE_FREMEN_AREA, 2, allowed);
	return allowed;
}

bool Arrakis::fremenShipArea(AreaId id)
{
	static const std::vector<AreaId> allowed = initFremenShipArea();
	return std::find(allowed.cbegin(), allowed.cend(), id) != allowed.cend();
}

void Arrakis::place(Faction from, Placement what, bool hostile)
{
	place(what.where, PlacedForces{from, what.normal, what.special, hostile});
}

void Arrakis::placeHostile(Faction from, Placement what)
{
	place(what.where, PlacedForces{from, what.normal, what.special});
}

void Arrakis::placeNeutral(Faction from, Placement what)
{
	place(what.where, PlacedForces{from, what.normal, what.special, false});
}

void Arrakis::removeForces(Faction from, Placement what)
{
	auto& placements = getOrCreate(what.where);
	placements.removeForces(PlacedForces{from, what.normal, what.special});
}

bool Arrakis::setTerritoryHostility(Faction who, Territory where, bool value)
{
	bool changed = false;

	where.forEach([&](AreaId area) {

		auto& placements = getOrCreate(area);
		placements.setHostile(who);

		changed = true;
	});

	return true;
}

int Arrakis::getStorm() const noexcept
{
	return storm;
}

bool Arrakis::insideStorm(AreaId id) const
{
	return Arrakis::insideStorm(id, storm);
}

static std::vector<FactionPosition>::iterator
firstPlayer(std::vector<FactionPosition>& v, int storm) noexcept
{
	const auto it = std::upper_bound(v.begin(), v.end(), FactionPosition{storm, Faction::any()});
	if (it == v.cend())
		return v.begin();
	else
		return it;
}

FactionPosition Arrakis::firstByStormOrder() const noexcept
{
	return mStormOrder.front();
}

FactionPosition Arrakis::lastByStormOrder() const noexcept
{
	return mStormOrder.back();
}

Faction Arrakis::firstOf(Faction factions) const noexcept
{
	for (const auto& f: mStormOrder)
	{
		if (factions.contains(f.faction))
			return f.faction;
	}

	return Faction::none();
}

Faction Arrakis::lastOf(Faction factions) const noexcept
{
	auto it = mStormOrder.crbegin();

	for (; it != mStormOrder.crend(); ++it)
	{
		if (factions.contains(it->faction))
			return it->faction;
	}

	return Faction::none();
}

Conflicts Arrakis::createConflicts() const
{
	auto areas = dividedTerritories();
	auto conflicted = contestedTerritories(areas);
	return Conflicts(std::move(conflicted), mStormOrder);
}

std::vector<FactionPosition> Arrakis::stormOrder() const
{
	return std::vector<FactionPosition>(mStormOrder.cbegin(), mStormOrder.cend());
}

void Arrakis::updateStormOrder()
{
	std::rotate(mStormOrder.begin(), firstPlayer(mStormOrder, storm), mStormOrder.end());
}

int Arrakis::setStorm(int position)
{
	storm = position >= 1 ? position : 1;
	return advanceStorm(0);
}

int Arrakis::advanceStorm(int count)
{
	storm += count;
	storm = ((storm - 1) % 18) + 1;
	updateStormOrder();
	return storm;
}

static std::vector<std::pair<AreaId, int>>::iterator findSpice(AreaId id,
		std::vector<std::pair<AreaId, int>>& source) noexcept
{
	return std::find_if(source.begin(), source.end(),
			[id](const auto& v) { return v.first == id; });
}


int Arrakis::addSpice(AreaId area, int amount)
{
	auto it = findSpice(area, spice);
	if (it != spice.end())
	{
		(it->second) += amount;
		return it->second;
	}
	else
	{
		spice.emplace_back(area, amount);
		return amount;
	}
}

int Arrakis::removeSpice(AreaId area, int amount) noexcept
{
	auto it = findSpice(area, spice);

	if (it == spice.end())
		return 0;

	int& available = it->second;

	if (available <= amount)
	{
		spice.erase(it);
		return 0;
	}
	else
	{
		available -= amount;
		return available;
	}
}

int Arrakis::getSpice(AreaId area) const noexcept
{
	auto it = findSpice(area, const_cast<std::vector<std::pair<AreaId, int>>&> (spice));
	return it == spice.end() ? 0 : it->second;
}

ForcesInArea& Arrakis::getOrCreate(AreaId area)
{
	auto it = std::find_if(mPlacements.begin(), mPlacements.end(), [&](const ForcesInArea& el)
	{
		return el.area().contains(area);
	});

	if (it != mPlacements.end())
		return *it;

	mPlacements.push_back(ForcesInArea(PartialTerritory(area)));
	return mPlacements.back();
}

std::vector<PartialTerritory> Arrakis::dividedTerritories() const
{
	std::vector<PartialTerritory> result;
	result.reserve(Territories::all().size() + 5);

	std::for_each(Territories::all().cbegin(), Territories::all().cend(), [&](const Territory& t) {

		auto pair = t.divideBy(StormPosition(storm));
		result.push_back(pair.first);

		if (!pair.second.empty()) {
			gLog->debug("divided territory %s,%s", pair.first.print().c_str(), pair.second.print().c_str());
			result.push_back(pair.second);
		}
	});

	return result;
}

std::vector<ForcesInArea> Arrakis::contestedTerritories(const std::vector<PartialTerritory>& areas) const
{
	std::vector<ForcesInArea> result;

	std::for_each(areas.cbegin(), areas.cend(), [&](const PartialTerritory& t){

		if (hostileFactions(t) > 1)
			result.push_back(queryArea(t));
	});

	return result;
}

void Arrakis::place(AreaId where, const PlacedForces& what)
{
	auto& placements = getOrCreate(where);
	placements.addForces(what);
}

bool Arrakis::isOccupied(Faction who, AreaId where, bool moveAsHostiles) const
{
	if (!isStronghold(where))
		return false;

	if (!moveAsHostiles)
		return false;

	return hostileEnemies(who, Territories::of(where)) >= 2;
}

int Arrakis::movementRange(Faction who) const noexcept
{
	if (hasMovementBonus(who))
		return 3;
	else if (who == Faction::fremen())
		return 2;
	else
		return 1;
}

bool Arrakis::hasMovementBonus(Faction who) const noexcept
{
	return queryArea(Territories::carthag).hasHostileForces(who) ||
		   queryArea(Territories::arrakeen).hasHostileForces(who);
}


bool Arrakis::canShip(Faction who, AreaId where) const
{
	if (who == Faction::fremen() && !fremenShipArea(where))
		return false;
	if (insideStorm(where, storm))
		return false;
	if (isOccupied(who, where))
		return false;
	return true;
}

bool Arrakis::canMove(Faction who, AreaId from, AreaId to, bool moveAsHostiles)
{
	if (!isReachable(from, to, movementRange(who)))
		return false;
	if (isOccupied(who, to, moveAsHostiles))
		return false;
	return true;
}

ForcesInArea Arrakis::forcesInArea(const PartialTerritory& where) const
{
	return queryArea(where);
}

int Arrakis::hostileFactions(const PartialTerritory& where) const
{
	auto forces = queryArea(where);

	return forces.countFactionIf([](const PlacedForces& el){
		return el.hostile;
	});
}

int Arrakis::hostileEnemies(Faction own, const PartialTerritory& where) const
{
	auto forces = queryArea(where);

	return forces.countFactionIf([own](const PlacedForces& el){
		return el.hostile && el.faction != own;
	});
}

int Arrakis::neutralFactions(const PartialTerritory& where) const
{
	auto forces = queryArea(where);

	return forces.countFactionIf([](const PlacedForces& el){
		return !el.hostile;
	});
}

ForcesInArea Arrakis::queryArea(PartialTerritory target) const
{
	ForcesInArea result;
	std::for_each(mPlacements.cbegin(), mPlacements.cend(), [&](const ForcesInArea& el){
		if (target.containsAllOf(el.area()))
			result.merge(el);
	});

	return result;
}


const std::vector<Arrakis::Connection> Arrakis::connections = {
		{ PolarSink              , CielagoNorth_1       },
		{ PolarSink              , CielagoNorth_2       },
		{ PolarSink              , CielagoNorth_3       },
		{ PolarSink              , HargPass_4           },
		{ PolarSink              , FalseWallEast_5      },
		{ PolarSink              , FalseWallEast_6      },
		{ PolarSink              , FalseWallEast_7      },
		{ PolarSink              , FalseWallEast_8      },
		{ PolarSink              , FalseWallEast_9      },
		{ PolarSink              , ImperialBasin_9      },
		{ PolarSink              , ImperialBasin_10     },
		{ PolarSink              , Arsunt_11            },
		{ PolarSink              , Arsunt_12            },
		{ PolarSink              , HaggaBasin_13        },
		{ PolarSink              , WindPass_14          },
		{ PolarSink              , WindPass_15          },
		{ PolarSink              , WindPass_16          },
		{ PolarSink              , WindPassNorth_17     },
		{ PolarSink              , WindPassNorth_18     },

		{ FalseWallEast_5        , PolarSink            },
		{ FalseWallEast_5        , HargPass_4           },
		{ FalseWallEast_5        , HargPass_5           },
		{ FalseWallEast_5        , TheMinorErg_5        },
		{ FalseWallEast_5        , FalseWallEast_6      },

		{ FalseWallEast_6        , PolarSink            },
		{ FalseWallEast_6        , FalseWallEast_5      },
		{ FalseWallEast_6        , TheMinorErg_6        },
		{ FalseWallEast_6        , FalseWallEast_7      },

		{ FalseWallEast_7        , PolarSink            },
		{ FalseWallEast_7        , FalseWallEast_6      },
		{ FalseWallEast_7        , TheMinorErg_7        },
		{ FalseWallEast_7        , FalseWallEast_8      },

		{ FalseWallEast_8        , PolarSink            },
		{ FalseWallEast_8        , FalseWallEast_7      },
		{ FalseWallEast_8        , FalseWallEast_9      },
		{ FalseWallEast_8        , TheMinorErg_8        },
		{ FalseWallEast_8        , ShieldWall_8         },

		{ FalseWallEast_9        , PolarSink            },
		{ FalseWallEast_9        , FalseWallEast_8      },
		{ FalseWallEast_9        , ImperialBasin_9      },
		{ FalseWallEast_9        , ShieldWall_9         },

		{ TheMinorErg_5          , FalseWallEast_5      },
		{ TheMinorErg_5          , HargPass_5           },
		{ TheMinorErg_5          , FalseWallSouth_5     },
		{ TheMinorErg_5          , PastyMesa_5          },
		{ TheMinorErg_5          , TheMinorErg_6        },

		{ TheMinorErg_6          , FalseWallEast_6      },
		{ TheMinorErg_6          , PastyMesa_6          },
		{ TheMinorErg_6          , TheMinorErg_5        },
		{ TheMinorErg_6          , TheMinorErg_7        },

		{ TheMinorErg_7          , FalseWallEast_7      },
		{ TheMinorErg_7          , PastyMesa_7          },
		{ TheMinorErg_7          , TheMinorErg_6        },
		{ TheMinorErg_7          , TheMinorErg_8        },

		{ TheMinorErg_8          , FalseWallEast_8      },
		{ TheMinorErg_8          , PastyMesa_8          },
		{ TheMinorErg_8          , TheMinorErg_7        },
		{ TheMinorErg_8          , ShieldWall_8         },

		{ PastyMesa_5            , TheMinorErg_5        },
		{ PastyMesa_5            , FalseWallSouth_5     },
		{ PastyMesa_5            , TueksSietch          },
		{ PastyMesa_5            , SouthMesa_5          },
		{ PastyMesa_5            , PastyMesa_6          },

		{ PastyMesa_6            , PastyMesa_5          },
		{ PastyMesa_6            , PastyMesa_7          },
		{ PastyMesa_6            , TheMinorErg_6        },
		{ PastyMesa_6            , SouthMesa_6          },

		{ PastyMesa_7            , PastyMesa_8          },
		{ PastyMesa_7            , PastyMesa_6          },
		{ PastyMesa_7            , TheMinorErg_7        },
		{ PastyMesa_7            , RedChasm             },

		{ PastyMesa_8            , PastyMesa_7          },
		{ PastyMesa_8            , TheMinorErg_8        },
		{ PastyMesa_8            , ShieldWall_8         },
		{ PastyMesa_8            , GaraKulon            },

		{ RedChasm               , PastyMesa_7          },
		{ RedChasm               , SouthMesa_6          },

		{ ShieldWall_8           , ShieldWall_9         },
		{ ShieldWall_8           , FalseWallEast_8      },
		{ ShieldWall_8           , TheMinorErg_8        },
		{ ShieldWall_8           , PastyMesa_8          },
		{ ShieldWall_8           , GaraKulon            },

		{ ShieldWall_9           , ShieldWall_8         },
		{ ShieldWall_9           , FalseWallEast_9      },
		{ ShieldWall_9           , ImperialBasin_9      },
		{ ShieldWall_9           , HoleInTheRock        },
		{ ShieldWall_9           , SihayaRidge          },

		{ SihayaRidge            , GaraKulon            },
		{ SihayaRidge            , ShieldWall_9         },
		{ SihayaRidge            , HoleInTheRock        },
		{ SihayaRidge            , Basin                },

		{ HoleInTheRock          , ShieldWall_9         },
		{ HoleInTheRock          , ImperialBasin_9      },
		{ HoleInTheRock          , RimWallWest          },
		{ HoleInTheRock          , Basin                },
		{ HoleInTheRock          , SihayaRidge          },

		{ Basin                  , SihayaRidge          },
		{ Basin                  , HoleInTheRock        },
		{ Basin                  , RimWallWest          },
		{ Basin                  , OldGap_9             },

		{ RimWallWest            , Arrakeen             },
		{ RimWallWest            , OldGap_9             },
		{ RimWallWest            , Basin                },
		{ RimWallWest            , HoleInTheRock        },
		{ RimWallWest            , ImperialBasin_9      },
		{ RimWallWest            , ImperialBasin_10     },

	    { ImperialBasin_9        , PolarSink            },
		{ ImperialBasin_9        , ImperialBasin_10     },
		{ ImperialBasin_9        , FalseWallEast_9      },
		{ ImperialBasin_9        , ShieldWall_9         },
		{ ImperialBasin_9        , HoleInTheRock        },
		{ ImperialBasin_9        , RimWallWest          },

		{ ImperialBasin_10       , ImperialBasin_9      },
		{ ImperialBasin_10       , RimWallWest          },
		{ ImperialBasin_10       , Arrakeen             },
		{ ImperialBasin_10       , OldGap_10            },
		{ ImperialBasin_10       , ImperialBasin_11     },
		{ ImperialBasin_10       , Arsunt_11            },
		{ ImperialBasin_10       , PolarSink            },

		{ ImperialBasin_11       , ImperialBasin_10     },
		{ ImperialBasin_11       , Arsunt_11            },
		{ ImperialBasin_11       , Carthag              },
		{ ImperialBasin_11       , Tsimpo_11            },

		{ GaraKulon              , PastyMesa_8          },
		{ GaraKulon              , ShieldWall_8         },
		{ GaraKulon              , SihayaRidge          },

		{ Arrakeen               , ImperialBasin_10     },
		{ Arrakeen               , OldGap_10            },
		{ Arrakeen               , RimWallWest          },

		{ OldGap_9               , OldGap_10            },
		{ OldGap_9               , RimWallWest          },
		{ OldGap_9               , Basin                },

		{ OldGap_10              , OldGap_9             },
		{ OldGap_10              , Arrakeen             },
		{ OldGap_10              , ImperialBasin_10     },
		{ OldGap_10              , OldGap_11            },

		{ OldGap_11              , OldGap_10            },
		{ OldGap_11              , Tsimpo_11            },
		{ OldGap_11              , BrokenLand_11        },

		{ Carthag                , ImperialBasin_11     },
		{ Carthag                , Arsunt_11            },
		{ Carthag                , HaggaBasin_12        },
		{ Carthag                , Tsimpo_12            },
		{ Carthag                , Tsimpo_11            },

		{ Tsimpo_11              , BrokenLand_11        },
		{ Tsimpo_11              , OldGap_11            },
		{ Tsimpo_11              , ImperialBasin_11     },
		{ Tsimpo_11              , Carthag              },
		{ Tsimpo_11              , Tsimpo_12            },

		{ Tsimpo_12              , Tsimpo_11            },
		{ Tsimpo_12              , Carthag              },
		{ Tsimpo_12              , HaggaBasin_12        },
		{ Tsimpo_12              , Tsimpo_13            },
		{ Tsimpo_12              , PlasticBasin_12      },
		{ Tsimpo_12              , BrokenLand_12        },

		{ Tsimpo_13              , Tsimpo_12            },
		{ Tsimpo_13              , HaggaBasin_13        },
		{ Tsimpo_13              , PlasticBasin_13      },

		{ BrokenLand_11          , BrokenLand_12        },
		{ BrokenLand_11          , OldGap_11            },
		{ BrokenLand_11          , Tsimpo_11            },

		{ BrokenLand_12          , BrokenLand_11        },
		{ BrokenLand_12          , Tsimpo_12            },
		{ BrokenLand_12          , PlasticBasin_12      },
		{ BrokenLand_12          , RockOutcroppings_13  },

		{ Arsunt_11              , PolarSink            },
		{ Arsunt_11              , Arsunt_12            },
		{ Arsunt_11              , HaggaBasin_12        },
		{ Arsunt_11              , Carthag              },
		{ Arsunt_11              , ImperialBasin_11     },
		{ Arsunt_11              , ImperialBasin_10     },

		{ Arsunt_12              , Arsunt_11            },
		{ Arsunt_12              , PolarSink            },
		{ Arsunt_12              , HaggaBasin_12        },
		{ Arsunt_12              , HaggaBasin_13        },

		{ PlasticBasin_12        , PlasticBasin_13      },
		{ PlasticBasin_12        , BrokenLand_12        },
		{ PlasticBasin_12        , Tsimpo_12            },

		{ PlasticBasin_13        , PlasticBasin_12      },
		{ PlasticBasin_13        , Tsimpo_13            },
		{ PlasticBasin_13        , HaggaBasin_13        },
		{ PlasticBasin_13        , PlasticBasin_14      },
		{ PlasticBasin_13        , RockOutcroppings_13  },

		{ PlasticBasin_14        , PlasticBasin_13      },
		{ PlasticBasin_14        , HaggaBasin_13        },
		{ PlasticBasin_14        , WindPass_14          },
		{ PlasticBasin_14        , TheGreatFlat         },
		{ PlasticBasin_14        , FuneralPlain         },
		{ PlasticBasin_14        , BightOfTheCliff_14   },
		{ PlasticBasin_14        , SietchTabr           },
		{ PlasticBasin_14        , RockOutcroppings_14  },

		{ HaggaBasin_12          , HaggaBasin_13        },
		{ HaggaBasin_12          , Arsunt_12            },
		{ HaggaBasin_12          , Arsunt_11            },
		{ HaggaBasin_12          , Carthag              },
		{ HaggaBasin_12          , Tsimpo_12            },

		{ HaggaBasin_13          , HaggaBasin_12        },
		{ HaggaBasin_13          , Tsimpo_13            },
		{ HaggaBasin_13          , PlasticBasin_13      },
		{ HaggaBasin_13          , PlasticBasin_14      },
		{ HaggaBasin_13          , WindPass_14          },
		{ HaggaBasin_13          , PolarSink            },
		{ HaggaBasin_13          , Arsunt_12            },

		{ RockOutcroppings_13    , PlasticBasin_13      },
		{ RockOutcroppings_13    , RockOutcroppings_14  },
		{ RockOutcroppings_13    , BrokenLand_12        },

		{ RockOutcroppings_14    , RockOutcroppings_13  },
		{ RockOutcroppings_14    , PlasticBasin_14      },
		{ RockOutcroppings_14    , SietchTabr           },
		{ RockOutcroppings_14    , BightOfTheCliff_14   },

		{ WindPass_14            , PlasticBasin_14      },
		{ WindPass_14            , PolarSink            },
		{ WindPass_14            , HaggaBasin_13        },
		{ WindPass_14            , WindPass_15          },

		{ WindPass_15            , PolarSink            },
		{ WindPass_15            , WindPass_14          },
		{ WindPass_15            , WindPass_16          },
		{ WindPass_15            , TheGreatFlat         },

		{ WindPass_16            , PolarSink            },
		{ WindPass_16            , WindPass_15          },
		{ WindPass_16            , TheGreaterFlat       },
		{ WindPass_16            , FalseWallWest_16     },
		{ WindPass_16            , WindPass_17          },
		{ WindPass_16            , WindPassNorth_17     },

		{ WindPass_17            , WindPass_16          },
		{ WindPass_17            , FalseWallWest_17     },
		{ WindPass_17            , CielagoWest_18       },
		{ WindPass_17            , WindPassNorth_17     },

		{ SietchTabr             , PlasticBasin_14      },
		{ SietchTabr             , RockOutcroppings_14  },
		{ SietchTabr             , BightOfTheCliff_14   },

		{ BightOfTheCliff_14     , RockOutcroppings_14  },
		{ BightOfTheCliff_14     , SietchTabr           },
		{ BightOfTheCliff_14     , PlasticBasin_14      },
		{ BightOfTheCliff_14     , BightOfTheCliff_15   },

		{ BightOfTheCliff_15     , BightOfTheCliff_14   },
		{ BightOfTheCliff_15     , FuneralPlain         },

		{ FuneralPlain           , BightOfTheCliff_15   },
		{ FuneralPlain           , PlasticBasin_14      },
		{ FuneralPlain           , TheGreatFlat         },

		{ TheGreatFlat           , FuneralPlain         },
		{ TheGreatFlat           , PlasticBasin_14      },
		{ TheGreatFlat           , WindPass_15          },
		{ TheGreatFlat           , TheGreaterFlat       },

		{ TheGreaterFlat         , TheGreatFlat         },
		{ TheGreaterFlat         , HabbanyaErg_16       },
		{ TheGreaterFlat         , FalseWallWest_16     },
		{ TheGreaterFlat         , WindPass_16          },

		{ HabbanyaErg_16         , TheGreaterFlat       },
		{ HabbanyaErg_16         , HabbanyaErg_17       },
		{ HabbanyaErg_16         , HabbanyaRidgeFlat_17 },

		{ HabbanyaErg_17         , HabbanyaErg_16       },
		{ HabbanyaErg_17         , HabbanyaRidgeFlat_17 },
		{ HabbanyaErg_17         , FalseWallWest_17     },

		{ FalseWallWest_16       , TheGreaterFlat       },
		{ FalseWallWest_16       , FalseWallWest_17     },
		{ FalseWallWest_16       , WindPass_16          },

		{ FalseWallWest_17       , FalseWallWest_16     },
		{ FalseWallWest_17       , HabbanyaErg_17       },
		{ FalseWallWest_17       , HabbanyaRidgeFlat_17 },
		{ FalseWallWest_17       , FalseWallWest_18     },
		{ FalseWallWest_17       , WindPass_17          },

		{ FalseWallWest_18       , FalseWallWest_17     },
		{ FalseWallWest_18       , HabbanyaRidgeFlat_18 },
		{ FalseWallWest_18       , CielagoWest_18       },

		{ HabbanyaRidgeFlat_17   , HabbanyaErg_16       },
		{ HabbanyaRidgeFlat_17   , HabbanyaErg_17       },
		{ HabbanyaRidgeFlat_17   , FalseWallWest_17     },
		{ HabbanyaRidgeFlat_17   , HabbanyaRidgeFlat_18 },
		{ HabbanyaRidgeFlat_17   , HabbanyaSietch       },

		{ HabbanyaRidgeFlat_18   , HabbanyaRidgeFlat_17 },
		{ HabbanyaRidgeFlat_18   , HabbanyaSietch       },
		{ HabbanyaRidgeFlat_18   , Meridian_1           },
		{ HabbanyaRidgeFlat_18   , CielagoWest_18       },
		{ HabbanyaRidgeFlat_18   , FalseWallWest_18     },

		{ CielagoWest_18         , CielagoWest_1        },
		{ CielagoWest_18         , CielagoNorth_1       },
		{ CielagoWest_18         , WindPassNorth_18     },
		{ CielagoWest_18         , WindPass_17          },
		{ CielagoWest_18         , FalseWallWest_18     },
		{ CielagoWest_18         , HabbanyaRidgeFlat_18 },

		{ CielagoWest_1          , CielagoWest_18       },
		{ CielagoWest_1          , Meridian_1           },
		{ CielagoWest_1          , CielagoDepression_1  },
		{ CielagoWest_1          , CielagoNorth_1       },

		{ WindPassNorth_17       , WindPass_16          },
		{ WindPassNorth_17       , WindPass_17          },
		{ WindPassNorth_17       , WindPassNorth_18     },
		{ WindPassNorth_17       , PolarSink            },

		{ WindPassNorth_18       , WindPassNorth_17     },
		{ WindPassNorth_18       , PolarSink            },
		{ WindPassNorth_18       , CielagoNorth_1       },
		{ WindPassNorth_18       , CielagoWest_18       },

		{ CielagoNorth_1         , PolarSink            },
		{ CielagoNorth_1         , WindPassNorth_18     },
		{ CielagoNorth_1         , CielagoWest_18       },
		{ CielagoNorth_1         , CielagoWest_1        },
		{ CielagoNorth_1         , CielagoDepression_1  },
		{ CielagoNorth_1         , CielagoNorth_2       },

		{ CielagoNorth_2         , CielagoNorth_1       },
		{ CielagoNorth_2         , CielagoNorth_3       },
		{ CielagoNorth_2         , PolarSink            },
		{ CielagoNorth_2         , CielagoDepression_2  },

		{ CielagoNorth_3         , CielagoNorth_2       },
		{ CielagoNorth_3         , PolarSink            },
		{ CielagoNorth_3         , HargPass_4           },
		{ CielagoNorth_3         , FalseWallSouth_4     },
		{ CielagoNorth_3         , CielagoEast_3        },
		{ CielagoNorth_3         , CielagoDepression_3  },

		{ HargPass_4             , PolarSink            },
		{ HargPass_4             , CielagoNorth_3       },
		{ HargPass_4             , FalseWallSouth_4     },
		{ HargPass_4             , HargPass_5           },
		{ HargPass_4             , FalseWallEast_5      },

		{ HargPass_5             , HargPass_4           },
		{ HargPass_5             , FalseWallEast_5      },
		{ HargPass_5             , TheMinorErg_5        },
		{ HargPass_5             , FalseWallSouth_5     },

		{ HabbanyaSietch         , HabbanyaRidgeFlat_17 },
		{ HabbanyaSietch         , HabbanyaRidgeFlat_18 },

		{ FalseWallSouth_4       , FalseWallSouth_5     },
		{ FalseWallSouth_4       , SouthMesa_4          },
		{ FalseWallSouth_4       , CielagoEast_4        },
		{ FalseWallSouth_4       , CielagoNorth_3       },
		{ FalseWallSouth_4       , HargPass_4           },

		{ FalseWallSouth_5       , FalseWallSouth_4     },
		{ FalseWallSouth_5       , HargPass_5           },
		{ FalseWallSouth_5       , TheMinorErg_5        },
		{ FalseWallSouth_5       , PastyMesa_5          },
		{ FalseWallSouth_5       , TueksSietch          },
		{ FalseWallSouth_5       , SouthMesa_5          },

		{ Meridian_1             , HabbanyaRidgeFlat_18 },
		{ Meridian_1             , CielagoWest_1        },
		{ Meridian_1             , CielagoDepression_1  },
		{ Meridian_1             , Meridian_2           },

		{ Meridian_2             , Meridian_1           },
		{ Meridian_2             , CielagoDepression_2  },
		{ Meridian_2             , CielagoSouth_2       },

		{ CielagoDepression_1    , CielagoDepression_2  },
		{ CielagoDepression_1    , Meridian_1           },
		{ CielagoDepression_1    , CielagoWest_1        },
		{ CielagoDepression_1    , CielagoNorth_1       },

		{ CielagoDepression_2    , CielagoDepression_1  },
		{ CielagoDepression_2    , CielagoDepression_3  },
		{ CielagoDepression_2    , CielagoNorth_2       },
		{ CielagoDepression_2    , CielagoSouth_2       },
		{ CielagoDepression_2    , Meridian_2           },

		{ CielagoDepression_3    , CielagoDepression_2  },
		{ CielagoDepression_3    , CielagoSouth_2       },
		{ CielagoDepression_3    , CielagoEast_3        },
		{ CielagoDepression_3    , CielagoNorth_3       },

		{ CielagoSouth_2         , Meridian_2           },
		{ CielagoSouth_2         , CielagoSouth_3       },
		{ CielagoSouth_2         , CielagoDepression_2  },

		{ CielagoSouth_3         , CielagoSouth_2       },
		{ CielagoSouth_3         , CielagoDepression_3  },
		{ CielagoSouth_3         , CielagoEast_3        },

		{ CielagoEast_3          , CielagoEast_4        },
		{ CielagoEast_3          , CielagoNorth_3       },
		{ CielagoEast_3          , CielagoDepression_3  },
		{ CielagoEast_3          , CielagoSouth_3       },

		{ CielagoEast_4          , CielagoEast_3        },
		{ CielagoEast_4          , FalseWallSouth_4     },
		{ CielagoEast_4          , SouthMesa_4          },

		{ SouthMesa_4            , SouthMesa_5          },
		{ SouthMesa_4            , FalseWallSouth_4     },
		{ SouthMesa_4            , CielagoEast_4        },

		{ SouthMesa_5            , SouthMesa_4          },
		{ SouthMesa_5            , SouthMesa_6          },
		{ SouthMesa_5            , FalseWallSouth_5     },
		{ SouthMesa_5            , PastyMesa_5          },
		{ SouthMesa_5            , TueksSietch          },

		{ SouthMesa_6            , SouthMesa_5          },
		{ SouthMesa_6            , RedChasm             },
		{ SouthMesa_6            , PastyMesa_6          },

		{ TueksSietch            , FalseWallSouth_5     },
		{ TueksSietch            , PastyMesa_5          },
		{ TueksSietch            , SouthMesa_5          }};
