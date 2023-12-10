#include "territories.h"

#include <algorithm>
#include <stdexcept>


const std::vector<Territory> Territories::msTerritories = {

		Territory(
	PolarSink),

		Territory(
	FalseWallEast_5        ,
	FalseWallEast_6        ,
	FalseWallEast_7        ,
	FalseWallEast_8        ,
	FalseWallEast_9),

		Territory(
	TheMinorErg_5          ,
	TheMinorErg_6          ,
	TheMinorErg_7          ,
	TheMinorErg_8),

		Territory(
	PastyMesa_5            ,
	PastyMesa_6            ,
	PastyMesa_7            ,
	PastyMesa_8),

		Territory(
	RedChasm),

		Territory(
	ShieldWall_8           ,
	ShieldWall_9),

		Territory(
	SihayaRidge),

		Territory(
	HoleInTheRock),

		Territory(
	Basin),

		Territory(
	RimWallWest),

		Territory(
	ImperialBasin_9        ,
	ImperialBasin_10       ,
	ImperialBasin_11),

		Territory(
	GaraKulon),

		Territory(
	Arrakeen),

		Territory(
	OldGap_9               ,
	OldGap_10              ,
	OldGap_11),

		Territory(
	Carthag),

		Territory(
	Tsimpo_11              ,
	Tsimpo_12              ,
	Tsimpo_13),

		Territory(
	BrokenLand_11          ,
	BrokenLand_12),

		Territory(
	Arsunt_11              ,
	Arsunt_12),

		Territory(
	PlasticBasin_12        ,
	PlasticBasin_13        ,
	PlasticBasin_14),


		Territory(
	HaggaBasin_12          ,
	HaggaBasin_13),

		Territory(
	RockOutcroppings_13    ,
	RockOutcroppings_14),

		Territory(
	WindPass_14            ,
	WindPass_15            ,
	WindPass_16            ,
	WindPass_17),

		Territory(
	SietchTabr),

		Territory(
	BightOfTheCliff_14     ,
	BightOfTheCliff_15),

		Territory(
	FuneralPlain),

		Territory(
	TheGreatFlat),

		Territory(
	TheGreaterFlat),

		Territory(
	HabbanyaErg_16         ,
	HabbanyaErg_17),

		Territory(
	FalseWallWest_16       ,
	FalseWallWest_17       ,
	FalseWallWest_18),

		Territory(
	HabbanyaRidgeFlat_17   ,
	HabbanyaRidgeFlat_18),

		Territory(
	CielagoWest_18         ,
	CielagoWest_1),

		Territory(
	WindPassNorth_17       ,
	WindPassNorth_18),

		Territory(
	CielagoNorth_1         ,
	CielagoNorth_2         ,
	CielagoNorth_3),

		Territory(
	HargPass_4             ,
	HargPass_5),

		Territory(
	HabbanyaSietch),

		Territory(
	FalseWallSouth_4       ,
	FalseWallSouth_5),

		Territory(
	Meridian_1             ,
	Meridian_2),

		Territory(
	CielagoDepression_1    ,
	CielagoDepression_2    ,
	CielagoDepression_3),

		Territory(
	CielagoSouth_2         ,
	CielagoSouth_3),

		Territory(
	CielagoEast_3          ,
	CielagoEast_4),

		Territory(
	SouthMesa_4            ,
	SouthMesa_5            ,
	SouthMesa_6),

		Territory(
	TueksSietch)
};


const std::vector<int> Territories::msSectors = {
		/*  PolarSink              */  0 ,
		/*  FalseWallEast_5        */  5 ,
		/*  FalseWallEast_6        */  6 ,
		/*  FalseWallEast_7        */  7 ,
		/*  FalseWallEast_8        */  8 ,
		/*  FalseWallEast_9        */  9 ,
		/*  TheMinorErg_5          */  5 ,
		/*  TheMinorErg_6          */  6 ,
		/*  TheMinorErg_7          */  7 ,
		/*  TheMinorErg_8          */  8 ,
		/*  PastyMesa_5            */  5 ,
		/*  PastyMesa_6            */  6 ,
		/*  PastyMesa_7            */  7 ,
		/*  PastyMesa_8            */  8 ,
		/*  RedChasm               */  7 ,
		/*  ShieldWall_8           */  8 ,
		/*  ShieldWall_9           */  9 ,
		/*  SihayaRidge            */  9 ,
		/*  HoleInTheRock          */  9 ,
		/*  Basin                  */  9 ,
		/*  RimWallWest            */  9 ,
		/*  ImperialBasin_9        */  9 ,
		/*  ImperialBasin_10       */  10,
		/*  ImperialBasin_11       */  11,
		/*  GaraKulon              */  8 ,
		/*  Arrakeen               */  10,
		/*  OldGap_9               */  9 ,
		/*  OldGap_10              */  10,
		/*  OldGap_11              */  11,
		/*  Carthag                */  11,
		/*  Tsimpo_11              */  11,
		/*  Tsimpo_12              */  12,
		/*  Tsimpo_13              */  13,
		/*  BrokenLand_11          */  11,
		/*  BrokenLand_12          */  12,
		/*  Arsunt_11              */  11,
		/*  Arsunt_12              */  12,
		/*  PlasticBasin_12        */  12,
		/*  PlasticBasin_13        */  13,
		/*  PlasticBasin_14        */  14,
		/*  HaggaBasin_12          */  12,
		/*  HaggaBasin_13          */  13,
		/*  RockOutcroppings_13    */  13,
		/*  RockOutcroppings_14    */  14,
		/*  WindPass_14            */  14,
		/*  WindPass_15            */  15,
		/*  WindPass_16            */  16,
		/*  WindPass_17            */  17,
		/*  SietchTabr             */  14,
		/*  BightOfTheCliff_14     */  14,
		/*  BightOfTheCliff_15     */  15,
		/*  FuneralPlain           */  15,
		/*  TheGreatFlat           */  15,
		/*  TheGreaterFlat         */  16,
		/*  HabbanyaErg_16         */  16,
		/*  HabbanyaErg_17         */  17,
		/*  FalseWallWest_16       */  16,
		/*  FalseWallWest_17       */  17,
		/*  FalseWallWest_18       */  18,
		/*  HabbanyaRidgeFlat_17   */  17,
		/*  HabbanyaRidgeFlat_18   */  18,
		/*  CielagoWest_18         */  18,
		/*  CielagoWest_1          */  1 ,
		/*  WindPassNorth_17       */  17,
		/*  WindPassNorth_18       */  18,
		/*  CielagoNorth_1         */  1 ,
		/*  CielagoNorth_2         */  2 ,
		/*  CielagoNorth_3         */  3 ,
		/*  HargPass_4             */  4 ,
		/*  HargPass_5             */  5 ,
		/*  HabbanyaSietch         */  17,
		/*  FalseWallSouth_4       */  4 ,
		/*  FalseWallSouth_5       */  5 ,
		/*  Meridian_1             */  1 ,
		/*  Meridian_2             */  2 ,
		/*  CielagoDepression_1    */  1 ,
		/*  CielagoDepression_2    */  2 ,
		/*  CielagoDepression_3    */  3 ,
		/*  CielagoSouth_2         */  2 ,
		/*  CielagoSouth_3         */  3 ,
		/*  CielagoEast_3          */  3 ,
		/*  CielagoEast_4          */  4 ,
		/*  SouthMesa_4            */  4 ,
		/*  SouthMesa_5            */  5 ,
		/*  SouthMesa_6            */  6 ,
		/*  TueksSietch            */  5
};

template<>
EnumAreaId::Definition EnumAreaId::entries = {
		{ PolarSink              , "PolarSink"              },
		{ FalseWallEast_5        , "FalseWallEast.5"        },
		{ FalseWallEast_6        , "FalseWallEast.6"        },
		{ FalseWallEast_7        , "FalseWallEast.7"        },
		{ FalseWallEast_8        , "FalseWallEast.8"        },
		{ FalseWallEast_9        , "FalseWallEast.9"        },
		{ TheMinorErg_5          , "TheMinorErg.5"          },
		{ TheMinorErg_6          , "TheMinorErg.6"          },
		{ TheMinorErg_7          , "TheMinorErg.7"          },
		{ TheMinorErg_8          , "TheMinorErg.8"          },
		{ PastyMesa_5            , "PastyMesa.5"            },
		{ PastyMesa_6            , "PastyMesa.6"            },
		{ PastyMesa_7            , "PastyMesa.7"            },
		{ PastyMesa_8            , "PastyMesa.8"            },
		{ RedChasm               , "RedChasm"               },
		{ ShieldWall_8           , "ShieldWall.8"           },
		{ ShieldWall_9           , "ShieldWall.9"           },
		{ SihayaRidge            , "SihayaRidge"            },
		{ HoleInTheRock          , "HoleInTheRock"          },
		{ Basin                  , "Basin"                  },
		{ RimWallWest            , "RimWallWest"            },
		{ ImperialBasin_9        , "ImperialBasin.9"        },
		{ ImperialBasin_10       , "ImperialBasin.10"       },
		{ ImperialBasin_11       , "ImperialBasin.11"       },
		{ GaraKulon              , "GaraKulon"              },
		{ Arrakeen               , "Arrakeen"               },
		{ OldGap_9               , "OldGap.9"               },
		{ OldGap_10              , "OldGap.10"              },
		{ OldGap_11              , "OldGap.11"              },
		{ Carthag                , "Carthag"                },
		{ Tsimpo_11              , "Tsimpo.11"              },
		{ Tsimpo_12              , "Tsimpo.12"              },
		{ Tsimpo_13              , "Tsimpo.13"              },
		{ BrokenLand_11          , "BrokenLand.11"          },
		{ BrokenLand_12          , "BrokenLand.12"          },
		{ Arsunt_11              , "Arsunt.11"              },
		{ Arsunt_12              , "Arsunt.12"              },
		{ PlasticBasin_12        , "PlasticBasin.12"        },
		{ PlasticBasin_13        , "PlasticBasin.13"        },
		{ PlasticBasin_14        , "PlasticBasin.14"        },
		{ HaggaBasin_12          , "HaggaBasin.12"          },
		{ HaggaBasin_13          , "HaggaBasin.13"          },
		{ RockOutcroppings_13    , "RockOutcroppingsarea.13"},
		{ RockOutcroppings_14    , "RockOutcroppings.14"    },
		{ WindPass_14            , "WindPass.14"            },
		{ WindPass_15            , "WindPass.15"            },
		{ WindPass_16            , "WindPass.16"            },
		{ WindPass_17            , "WindPass.17"            },
		{ SietchTabr             , "SietchTabr"             },
		{ BightOfTheCliff_14     , "BightOfTheCliff.14"     },
		{ BightOfTheCliff_15     , "BightOfTheCliff.15"     },
		{ FuneralPlain           , "FuneralPlain"           },
		{ TheGreatFlat           , "TheGreatFlat"           },
		{ TheGreaterFlat         , "TheGreaterFlat"         },
		{ HabbanyaErg_16         , "HabbanyaErg.16"         },
		{ HabbanyaErg_17         , "HabbanyaErg.17"         },
		{ FalseWallWest_16       , "FalseWallWest.16"       },
		{ FalseWallWest_17       , "FalseWallWest.17"       },
		{ FalseWallWest_18       , "FalseWallWest.18"       },
		{ HabbanyaRidgeFlat_17   , "HabbanyaRidgeFlat.17"   },
		{ HabbanyaRidgeFlat_18   , "HabbanyaRidgeFlat.18"   },
		{ CielagoWest_18         , "CielagoWest.18"         },
		{ CielagoWest_1          , "CielagoWest.1"          },
		{ WindPassNorth_17       , "WindPassNorth.17"       },
		{ WindPassNorth_18       , "WindPassNorth.18"       },
		{ CielagoNorth_1         , "CielagoNorth.1"         },
		{ CielagoNorth_2         , "CielagoNorth.2"         },
		{ CielagoNorth_3         , "CielagoNorth.3"         },
		{ HargPass_4             , "HargPass.4"             },
		{ HargPass_5             , "HargPass.5"             },
		{ HabbanyaSietch         , "HabbanyaSietch"         },
		{ FalseWallSouth_4       , "FalseWallSouth.4"       },
		{ FalseWallSouth_5       , "FalseWallSouth.5"       },
		{ Meridian_1             , "Meridian.1"             },
		{ Meridian_2             , "Meridian.2"             },
		{ CielagoDepression_1    , "CielagoDepression.1"    },
		{ CielagoDepression_2    , "CielagoDepression.2"    },
		{ CielagoDepression_3    , "CielagoDepression.3"    },
		{ CielagoSouth_2         , "CielagoSouth.2"         },
		{ CielagoSouth_3         , "CielagoSouth.3"         },
		{ CielagoEast_3          , "CielagoEast.3"          },
		{ CielagoEast_4          , "CielagoEast.4"          },
		{ SouthMesa_4            , "SouthMesa.4"            },
		{ SouthMesa_5            , "SouthMesa.5"            },
		{ SouthMesa_6            , "SouthMesa.6"            },
		{ TueksSietch            , "TueksSietch"		    }
};

const Territory& Territories::polarSink =                   msTerritories[0];
const Territory& Territories::falseWallEast =               msTerritories[1];
const Territory& Territories::theMinorErg =                 msTerritories[2];
const Territory& Territories::pastyMesa =                   msTerritories[3];
const Territory& Territories::redChasm =                    msTerritories[4];
const Territory& Territories::shieldWall =                  msTerritories[5];
const Territory& Territories::sihayaRidge =                 msTerritories[6];
const Territory& Territories::holeInTheRock =               msTerritories[7];
const Territory& Territories::basin =                       msTerritories[8];
const Territory& Territories::rimWallWest =                 msTerritories[9];
const Territory& Territories::imperialBasin =               msTerritories[10];
const Territory& Territories::garaKulon =                   msTerritories[11];
const Territory& Territories::arrakeen =                    msTerritories[12];
const Territory& Territories::oldGap =                      msTerritories[13];
const Territory& Territories::carthag =                     msTerritories[14];
const Territory& Territories::tsimpo =                      msTerritories[15];
const Territory& Territories::brokenLand =                  msTerritories[16];
const Territory& Territories::arsunt =                      msTerritories[17];
const Territory& Territories::plasticBasin =                msTerritories[18];
const Territory& Territories::haggaBasin =                  msTerritories[19];
const Territory& Territories::rockOutcroppings =            msTerritories[20];
const Territory& Territories::windPass =                    msTerritories[21];
const Territory& Territories::sietchTabr =                  msTerritories[22];
const Territory& Territories::bightOfTheCliff =             msTerritories[23];
const Territory& Territories::funeralPlain =                msTerritories[24];
const Territory& Territories::theGreatFlat =                msTerritories[25];
const Territory& Territories::theGreaterFlat =              msTerritories[26];
const Territory& Territories::habbanyaErg =                 msTerritories[27];
const Territory& Territories::falseWallWest =               msTerritories[28];
const Territory& Territories::habbanyaRidgeFlat =           msTerritories[29];
const Territory& Territories::cielagoWest =                 msTerritories[30];
const Territory& Territories::windPassNorth =               msTerritories[31];
const Territory& Territories::cielagoNorth =                msTerritories[32];
const Territory& Territories::hargPass =                    msTerritories[33];
const Territory& Territories::habbanyaSietch =              msTerritories[34];
const Territory& Territories::falseWallSouth =              msTerritories[35];
const Territory& Territories::meridian =                    msTerritories[36];
const Territory& Territories::cielagoDepression =           msTerritories[37];
const Territory& Territories::cielagoSouth =                msTerritories[38];
const Territory& Territories::cielagoEast =                 msTerritories[39];
const Territory& Territories::southMesa =                   msTerritories[40];
const Territory& Territories::tueksSietch =                 msTerritories[41];

const Territory& Territories::of(AreaId area)
{
	return of(PartialTerritory(area));
}

const Territory& Territories::of(const PartialTerritory& area)
{
	auto it = std::find_if(msTerritories.cbegin(), msTerritories.cend(), [&](const Territory& el){
		return el.overlaps(area);
	});

	if (it == msTerritories.cend())
		throw std::invalid_argument("area is not known");

	return *it;
}

const int Territories::sectorOf(AreaId area) noexcept
{
	static constexpr int NO_SECTOR = 0;

	auto it = std::find_if(msTerritories.cbegin(), msTerritories.cend(), [&](const Territory& t) {
		return t.contains(area);
	});

	if (it == msTerritories.cend())
		return NO_SECTOR;

	return msSectors[std::distance(msTerritories.cbegin(), it)];
}

