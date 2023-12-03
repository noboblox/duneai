#include "territories.h"

#include <algorithm>
#include <stdexcept>

const Territory Territories::polarSink = Territory(
	PolarSink
);

const Territory Territories::falseWallEast = Territory(
	FalseWallEast_5        ,
	FalseWallEast_6        ,
	FalseWallEast_7        ,
	FalseWallEast_8        ,
	FalseWallEast_9
);

const Territory Territories::theMinorErg = Territory(
	TheMinorErg_5          ,
	TheMinorErg_6          ,
	TheMinorErg_7          ,
	TheMinorErg_8
);

const Territory Territories::pastyMesa = Territory(
	PastyMesa_5            ,
	PastyMesa_6            ,
	PastyMesa_7            ,
	PastyMesa_8
);

const Territory Territories::redChasm = Territory(
	RedChasm
);

const Territory Territories::shieldWall = Territory(
	ShieldWall_8           ,
	ShieldWall_9
);

const Territory Territories::sihayaRidge = Territory(
	SihayaRidge
);

const Territory Territories::holeInTheRock = Territory(
	HoleInTheRock
);

const Territory Territories::basin = Territory(
	Basin
);

const Territory Territories::rimWallWest = Territory(
	RimWallWest
);

const Territory Territories::imperialBasin = Territory(
	ImperialBasin_9        ,
	ImperialBasin_10       ,
	ImperialBasin_11
);

const Territory Territories::garaKulon = Territory(
	GaraKulon
);

const Territory Territories::arrakeen = Territory(
	Arrakeen
);

const Territory Territories::oldGap = Territory(
	OldGap_9               ,
	OldGap_10              ,
	OldGap_11
);

const Territory Territories::carthag = Territory(
	Carthag
);

const Territory Territories::tsimpo = Territory(
	Tsimpo_11              ,
	Tsimpo_12              ,
	Tsimpo_13
);

const Territory Territories::brokenLand = Territory(
	BrokenLand_11          ,
	BrokenLand_12
);

const Territory Territories::arsunt = Territory(
	Arsunt_11              ,
	Arsunt_12
);

const Territory Territories::plasticBasin = Territory(
	PlasticBasin_12        ,
	PlasticBasin_13        ,
	PlasticBasin_14
);

const Territory Territories::haggaBasin = Territory(
	HaggaBasin_12          ,
	HaggaBasin_13
);

const Territory Territories::rockOutcroppings = Territory(
	RockOutcroppings_13    ,
	RockOutcroppings_14
);

const Territory Territories::windPass = Territory(
	WindPass_14            ,
	WindPass_15            ,
	WindPass_16            ,
	WindPass_17
);

const Territory Territories::sietchTabr = Territory(
	SietchTabr
);

const Territory Territories::bightOfTheCliff = Territory(
	BightOfTheCliff_14     ,
	BightOfTheCliff_15
);

const Territory Territories::funeralPlain = Territory(
	FuneralPlain
);

const Territory Territories::theGreatFlat = Territory(
	TheGreatFlat
);

const Territory Territories::theGreaterFlat = Territory(
	TheGreaterFlat
);

const Territory Territories::habbanyaErg = Territory(
	HabbanyaErg_16         ,
	HabbanyaErg_17
);

const Territory Territories::falseWallWest = Territory(
	FalseWallWest_16       ,
	FalseWallWest_17       ,
	FalseWallWest_18
);

const Territory Territories::habbanyaRidgeFlat = Territory(
	HabbanyaRidgeFlat_17   ,
	HabbanyaRidgeFlat_18
);

const Territory Territories::cielagoWest = Territory(
	CielagoWest_18         ,
	CielagoWest_1
);

const Territory Territories::windPassNorth = Territory(
	WindPassNorth_17       ,
	WindPassNorth_18
);

const Territory Territories::cielagoNorth = Territory(
	CielagoNorth_1         ,
	CielagoNorth_2         ,
	CielagoNorth_3
);

const Territory Territories::hargPass = Territory(
	HargPass_4             ,
	HargPass_5
);

const Territory Territories::habbanyaSietch = Territory(
	HabbanyaSietch
);

const Territory Territories::falseWallSouth = Territory(
	FalseWallSouth_4       ,
	FalseWallSouth_5
);

const Territory Territories::meridian = Territory(
	Meridian_1             ,
	Meridian_2
);

const Territory Territories::cielagoDepression = Territory(
	CielagoDepression_1    ,
	CielagoDepression_2    ,
	CielagoDepression_3
);

const Territory Territories::cielagoSouth = Territory(
	CielagoSouth_2         ,
	CielagoSouth_3
);

const Territory Territories::cielagoEast = Territory(
	CielagoEast_3          ,
	CielagoEast_4
);

const Territory Territories::southMesa = Territory(
	SouthMesa_4            ,
	SouthMesa_5            ,
	SouthMesa_6
);

const Territory Territories::tueksSietch = Territory(
	TueksSietch
);

const Territory& Territories::of(AreaId area)
{
	auto it = std::find_if(msAreaToTerritory.cbegin(), msAreaToTerritory.cend(), [&](TerritoryForArea el){
		return el.child == area;
	});

	if (it == msAreaToTerritory.cend())
		throw std::invalid_argument("area is not known");

	return *(it->parent);
}

const std::vector<Territories::TerritoryForArea> Territories::msAreaToTerritory = {
		TerritoryForArea{ PolarSink             ,  &Territories::polarSink        },
		TerritoryForArea{ FalseWallEast_5       ,  &Territories::falseWallEast    },
		TerritoryForArea{ FalseWallEast_6       ,  &Territories::falseWallEast    },
		TerritoryForArea{ FalseWallEast_7       ,  &Territories::falseWallEast    },
		TerritoryForArea{ FalseWallEast_8       ,  &Territories::falseWallEast    },
		TerritoryForArea{ FalseWallEast_9       ,  &Territories::falseWallEast    },
		TerritoryForArea{ TheMinorErg_5         ,  &Territories::theMinorErg      },
		TerritoryForArea{ TheMinorErg_6         ,  &Territories::theMinorErg      },
		TerritoryForArea{ TheMinorErg_7         ,  &Territories::theMinorErg      },
		TerritoryForArea{ TheMinorErg_8         ,  &Territories::theMinorErg      },
		TerritoryForArea{ PastyMesa_5           ,  &Territories::pastyMesa        },
		TerritoryForArea{ PastyMesa_6           ,  &Territories::pastyMesa        },
		TerritoryForArea{ PastyMesa_7           ,  &Territories::pastyMesa        },
		TerritoryForArea{ PastyMesa_8           ,  &Territories::pastyMesa        },
		TerritoryForArea{ RedChasm              ,  &Territories::redChasm         },
		TerritoryForArea{ ShieldWall_8          ,  &Territories::shieldWall       },
		TerritoryForArea{ ShieldWall_9          ,  &Territories::shieldWall       },
		TerritoryForArea{ SihayaRidge           ,  &Territories::sihayaRidge      },
		TerritoryForArea{ HoleInTheRock         ,  &Territories::holeInTheRock    },
		TerritoryForArea{ Basin                 ,  &Territories::basin            },
		TerritoryForArea{ RimWallWest           ,  &Territories::rimWallWest      },
		TerritoryForArea{ ImperialBasin_9       ,  &Territories::imperialBasin    },
		TerritoryForArea{ ImperialBasin_10      ,  &Territories::imperialBasin    },
		TerritoryForArea{ ImperialBasin_11      ,  &Territories::imperialBasin    },
		TerritoryForArea{ GaraKulon             ,  &Territories::garaKulon        },
		TerritoryForArea{ Arrakeen              ,  &Territories::arrakeen         },
		TerritoryForArea{ OldGap_9              ,  &Territories::oldGap           },
		TerritoryForArea{ OldGap_10             ,  &Territories::oldGap           },
		TerritoryForArea{ OldGap_11             ,  &Territories::oldGap           },
		TerritoryForArea{ Carthag               ,  &Territories::carthag          },
		TerritoryForArea{ Tsimpo_11             ,  &Territories::tsimpo           },
		TerritoryForArea{ Tsimpo_12             ,  &Territories::tsimpo           },
		TerritoryForArea{ Tsimpo_13             ,  &Territories::tsimpo           },
		TerritoryForArea{ BrokenLand_11         ,  &Territories::brokenLand       },
		TerritoryForArea{ BrokenLand_12         ,  &Territories::brokenLand       },
		TerritoryForArea{ Arsunt_11             ,  &Territories::arsunt           },
		TerritoryForArea{ Arsunt_12             ,  &Territories::arsunt           },
		TerritoryForArea{ PlasticBasin_12       ,  &Territories::plasticBasin     },
		TerritoryForArea{ PlasticBasin_13       ,  &Territories::plasticBasin     },
		TerritoryForArea{ PlasticBasin_14       ,  &Territories::plasticBasin     },
		TerritoryForArea{ HaggaBasin_12         ,  &Territories::haggaBasin       },
		TerritoryForArea{ HaggaBasin_13         ,  &Territories::haggaBasin       },
		TerritoryForArea{ RockOutcroppings_13   ,  &Territories::rockOutcroppings },
		TerritoryForArea{ RockOutcroppings_14   ,  &Territories::rockOutcroppings },
		TerritoryForArea{ WindPass_14           ,  &Territories::windPass         },
		TerritoryForArea{ WindPass_15           ,  &Territories::windPass         },
		TerritoryForArea{ WindPass_16           ,  &Territories::windPass         },
		TerritoryForArea{ WindPass_17           ,  &Territories::windPass         },
		TerritoryForArea{ SietchTabr            ,  &Territories::sietchTabr       },
		TerritoryForArea{ BightOfTheCliff_14    ,  &Territories::bightOfTheCliff  },
		TerritoryForArea{ BightOfTheCliff_15    ,  &Territories::bightOfTheCliff  },
		TerritoryForArea{ FuneralPlain          ,  &Territories::funeralPlain     },
		TerritoryForArea{ TheGreatFlat          ,  &Territories::theGreatFlat     },
		TerritoryForArea{ TheGreaterFlat        ,  &Territories::theGreaterFlat   },
		TerritoryForArea{ HabbanyaErg_16        ,  &Territories::habbanyaErg      },
		TerritoryForArea{ HabbanyaErg_17        ,  &Territories::habbanyaErg      },
		TerritoryForArea{ FalseWallWest_16      ,  &Territories::falseWallWest    },
		TerritoryForArea{ FalseWallWest_17      ,  &Territories::falseWallWest    },
		TerritoryForArea{ FalseWallWest_18      ,  &Territories::falseWallWest    },
		TerritoryForArea{ HabbanyaRidgeFlat_17  ,  &Territories::habbanyaRidgeFlat},
		TerritoryForArea{ HabbanyaRidgeFlat_18  ,  &Territories::habbanyaRidgeFlat},
		TerritoryForArea{ CielagoWest_18        ,  &Territories::cielagoWest      },
		TerritoryForArea{ CielagoWest_1         ,  &Territories::cielagoWest      },
		TerritoryForArea{ WindPassNorth_17      ,  &Territories::windPassNorth    },
		TerritoryForArea{ WindPassNorth_18      ,  &Territories::windPassNorth    },
		TerritoryForArea{ CielagoNorth_1        ,  &Territories::cielagoNorth     },
		TerritoryForArea{ CielagoNorth_2        ,  &Territories::cielagoNorth     },
		TerritoryForArea{ CielagoNorth_3        ,  &Territories::cielagoNorth     },
		TerritoryForArea{ HargPass_4            ,  &Territories::hargPass         },
		TerritoryForArea{ HargPass_5            ,  &Territories::hargPass         },
		TerritoryForArea{ HabbanyaSietch        ,  &Territories::habbanyaSietch   },
		TerritoryForArea{ FalseWallSouth_4      ,  &Territories::falseWallSouth   },
		TerritoryForArea{ FalseWallSouth_5      ,  &Territories::falseWallSouth   },
		TerritoryForArea{ Meridian_1            ,  &Territories::meridian         },
		TerritoryForArea{ Meridian_2            ,  &Territories::meridian         },
		TerritoryForArea{ CielagoDepression_1   ,  &Territories::cielagoDepression},
		TerritoryForArea{ CielagoDepression_2   ,  &Territories::cielagoDepression},
		TerritoryForArea{ CielagoDepression_3   ,  &Territories::cielagoDepression},
		TerritoryForArea{ CielagoSouth_2        ,  &Territories::cielagoSouth     },
		TerritoryForArea{ CielagoSouth_3        ,  &Territories::cielagoSouth     },
		TerritoryForArea{ CielagoEast_3         ,  &Territories::cielagoEast      },
		TerritoryForArea{ CielagoEast_4         ,  &Territories::cielagoEast      },
		TerritoryForArea{ SouthMesa_4           ,  &Territories::southMesa        },
		TerritoryForArea{ SouthMesa_5           ,  &Territories::southMesa        },
		TerritoryForArea{ SouthMesa_6           ,  &Territories::southMesa        },
		TerritoryForArea{ TueksSietch           ,  &Territories::tueksSietch      },
};
