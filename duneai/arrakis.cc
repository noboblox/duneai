/*
 * arrakis.cc
 *
 *  Created on: 26.11.2022
 *      Author: Rene
 */

#include "arrakis.h"
#include <algorithm>
#include <iterator>

const std::vector<Arrakis::Area> Arrakis::areas = {
		{ PolarSink              , "PolarSink"           , 0 },
		{ FalseWallEast_5        , "FalseWallEast.5"     , 5 },
		{ FalseWallEast_6        , "FalseWallEast.6"     , 6 },
		{ FalseWallEast_7        , "FalseWallEast.7"     , 7 },
		{ FalseWallEast_8        , "FalseWallEast.8"     , 8 },
		{ FalseWallEast_9        , "FalseWallEast.9"     , 9 },
		{ TheMinorErg_5          , "TheMinorErg.5"       , 5 },
		{ TheMinorErg_6          , "TheMinorErg.6"       , 6 },
		{ TheMinorErg_7          , "TheMinorErg.7"       , 7 },
		{ TheMinorErg_8          , "TheMinorErg.8"       , 8 },
		{ PastyMesa_5            , "PastyMesa.5"         , 5 },
		{ PastyMesa_6            , "PastyMesa.6"         , 6 },
		{ PastyMesa_7            , "PastyMesa.7"         , 7 },
		{ PastyMesa_8            , "PastyMesa.8"         , 8 },
		{ RedChasm               , "RedChasm"            , 7 },
		{ ShieldWall_8           , "ShieldWall.8"        , 8 },
		{ ShieldWall_9           , "ShieldWall.9"        , 9 },
		{ SihayaRidge            , "SihayaRidge"         , 9 },
		{ HoleInTheRock          , "HoleInTheRock"       , 9 },
		{ Basin                  , "Basin"               , 9 },
		{ RimWallWest            , "RimWallWest"         , 9 },
		{ ImperialBasin_9        , "ImperialBasin.9"     , 9 },
		{ ImperialBasin_10       , "ImperialBasin.10"    , 10},
		{ ImperialBasin_11       , "ImperialBasin.11"    , 11},
		{ GaraKulon              , "GaraKulon"           , 8 },
		{ Arrakeen               , "Arrakeen"            , 10},
		{ OldGap_9               , "OldGap.9"            , 9 },
		{ OldGap_10              , "OldGap.10"           , 10},
		{ OldGap_11              , "OldGap.11"           , 11},
		{ Carthag                , "Carthag"             , 11},
		{ Tsimpo_11              , "Tsimpo.11"           , 11},
		{ Tsimpo_12              , "Tsimpo.12"           , 12},
		{ Tsimpo_13              , "Tsimpo.13"           , 13},
		{ BrokenLand_11          , "BrokenLand.11"       , 11},
		{ BrokenLand_12          , "BrokenLand.12"       , 12},
		{ Arsunt_11              , "Arsunt.11"           , 11},
		{ Arsunt_12              , "Arsunt.12"           , 12},
		{ PlasticBasin_12        , "PlasticBasin.12"     , 12},
		{ PlasticBasin_13        , "PlasticBasin.13"     , 13},
		{ PlasticBasin_14        , "PlasticBasin.14"     , 14},
		{ HaggaBasin_12          , "HaggaBasin.12"       , 12},
		{ HaggaBasin_13          , "HaggaBasin.13"       , 13},
		{ RockOutcroppings_13    , "RockOutcroppings.13" , 13},
		{ RockOutcroppings_14    , "RockOutcroppings.14" , 14},
		{ WindPass_14            , "WindPass.14"         , 14},
		{ WindPass_15            , "WindPass.15"         , 15},
		{ WindPass_16            , "WindPass.16"         , 16},
		{ WindPass_17            , "WindPass.17"         , 17},
		{ SietchTabr             , "SietchTabr"          , 14},
		{ BightOfTheCliff_14     , "BightOfTheCliff.14"  , 14},
		{ BightOfTheCliff_15     , "BightOfTheCliff.15"  , 15},
		{ FuneralPlain           , "FuneralPlain"        , 15},
		{ TheGreatFlat           , "TheGreatFlat"        , 15},
		{ TheGreaterFlat         , "TheGreaterFlat"      , 16},
		{ HabbanyaErg_16         , "HabbanyaErg.16"      , 16},
		{ HabbanyaErg_17         , "HabbanyaErg.17"      , 17},
		{ FalseWallWest_16       , "FalseWallWest.16"    , 16},
		{ FalseWallWest_17       , "FalseWallWest.17"    , 17},
		{ FalseWallWest_18       , "FalseWallWest.18"    , 18},
		{ HabbanyaRidgeFlat_17   , "HabbanyaRidgeFlat.17", 17},
		{ HabbanyaRidgeFlat_18   , "HabbanyaRidgeFlat.18", 18},
		{ CielagoWest_18         , "CielagoWest.18"      , 18},
		{ CielagoWest_1          , "CielagoWest.1"       , 1 },
		{ WindPassNorth_17       , "WindPassNorth.17"    , 17},
		{ WindPassNorth_18       , "WindPassNorth.18"    , 18},
		{ CielagoNorth_1         , "CielagoNorth.1"      , 1 },
		{ CielagoNorth_2         , "CielagoNorth.2"      , 2 },
		{ CielagoNorth_3         , "CielagoNorth.3"      , 3 },
		{ HargPass_4             , "HargPass.4"          , 4 },
		{ HargPass_5             , "HargPass.5"          , 5 },
		{ HabbanyaSietch         , "HabbanyaSietch"      , 17},
		{ FalseWallSouth_4       , "FalseWallSouth.4"    , 4 },
		{ FalseWallSouth_5       , "FalseWallSouth.5"    , 5 },
		{ Meridian_1             , "Meridian.1"          , 1 },
		{ Meridian_2             , "Meridian.2"          , 2 },
		{ CielagoDepression_1    , "CielagoDepression.1" , 1 },
		{ CielagoDepression_2    , "CielagoDepression.2" , 2 },
		{ CielagoDepression_3    , "CielagoDepression.3" , 3 },
		{ CielagoSouth_2         , "CielagoSouth.2"      , 2 },
		{ CielagoSouth_3         , "CielagoSouth.3"      , 3 },
		{ CielagoEast_3          , "CielagoEast.3"       , 3 },
		{ CielagoEast_4          , "CielagoEast.4"       , 4 },
		{ SouthMesa_4            , "SouthMesa.4"         , 4 },
		{ SouthMesa_5            , "SouthMesa.5"         , 5 },
		{ SouthMesa_6            , "SouthMesa.6"         , 6 },
		{ TueksSietch            , "TueksSietch"		 , 5 }};


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

const char* Arrakis::areaName(AreaId id)
{
	const auto* pArea = getArea(id);

	if (!pArea)
		return "INVALID";
	else
		return pArea->name;
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

bool Arrakis::insideStorm(AreaId id, int storm)
{
	if (storm == 0)
		return false;

	auto p_area = getArea(id);

	if (!p_area)
		return true; // default true -> no movement into invalid areas

	return p_area->sector == storm;
}

bool Arrakis::isSietch(AreaId id)
{
	switch (id)
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

bool Arrakis::fremenInitArea(AreaId id)
{
	switch (id)
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

int Arrakis::areaSector(AreaId id)
{
	auto area = getArea(id);
	return area ? area->sector : 0;
}

int Arrakis::compareSector(int l, int r)
{
	if (l == r)
		return 0;

	int greaterFrom = ++r;
	if (greaterFrom > 18)  greaterFrom -= 18;

	int greaterUntil = r + 9;
	if (greaterUntil > 18)  greaterUntil -= 18;

	return l >= greaterFrom || l <= greaterUntil ? 1 : -1;
}

const Arrakis::Area* Arrakis::getArea(AreaId id)
{
	auto it = std::find_if(areas.cbegin(), areas.cend(),
			[id](const Area& a) { return a.id == id; });

	if (it == areas.cend())
		return nullptr;
	else
		return &(*it);
}

const Arrakis::Area* Arrakis::getTerritorySector(AreaId childArea, int sector)
{
	for (const auto& a : areas)
	{
		if (!sameTerritory(childArea, a.id))
			continue;
		if (a.sector != sector)
			continue;

		return &a;
	}
	return nullptr;
}

void Arrakis::place(Faction from, Placement source, bool hostile)
{
	place(ForcesFrom{from, source, hostile});
}

void Arrakis::placeHostile(Faction from, Placement source)
{
	place(ForcesFrom{from, source, true});
}

void Arrakis::placeNeutral(Faction from, Placement source)
{
	place(ForcesFrom{from, source, false});
}

void Arrakis::removeForces(Faction from, Placement source)
{
	auto it = std::find_if(mForces.begin(), mForces.end(),
			[&source, &from](const ForcesFrom& f)
			{ return f.from == from && f.where == source.where; });

	if (it == mForces.end())
		return;

	it->normal  -= source.normal;
	it->special -= source.special;

	if (it->normal <= 0 && it->special <= 0)
		mForces.erase(it);
}

bool Arrakis::setTerritoryHostility(Faction from, AreaId where, bool value)
{
	auto found = collectFromSameTerritory(where, &from, nullptr);

	if (!found.empty())
		const_cast<ForcesFrom*> (found.front())->hostile = value;
	return !found.empty();
}

int Arrakis::getStorm() const noexcept
{
	return storm;
}

static std::vector<Arrakis::FactionPosition>::iterator
firstPlayer(std::vector<Arrakis::FactionPosition>& v, int storm) noexcept
{
	const auto it = std::upper_bound(v.begin(), v.end(), Arrakis::FactionPosition{storm, Faction::any()});
	if (it == v.cend())
		return v.begin();
	else
		return it;
}

Arrakis::FactionPosition Arrakis::firstByStormOrder() const noexcept
{
	return mStormOrder.front();
}

Arrakis::FactionPosition Arrakis::lastByStormOrder() const noexcept
{
	return mStormOrder.back();
}

std::vector<Arrakis::FactionPosition> Arrakis::stormOrder() const
{
	return std::vector<FactionPosition>(mStormOrder.cbegin(), mStormOrder.cend());
}

void Arrakis::updateStormOrder()
{
	std::rotate(mStormOrder.begin(), firstPlayer(mStormOrder, storm), mStormOrder.end());
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

static bool FactionAndAreaEqual(const ForcesFrom& l, const ForcesFrom& r)
{
	return l.where == r.where && l.from == r.from;
}

static bool FactionAndAreaLess(const ForcesFrom& l, const ForcesFrom& r)
{
	if (l.where == r.where)
		return l.from < r.from;
	return l.where < r.where;
}

void Arrakis::place(ForcesFrom&& source)
{
	auto it = std::lower_bound(mForces.begin(), mForces.end(), source, FactionAndAreaLess);

	if (it == mForces.end() || !FactionAndAreaEqual(source, *it))
	{
		mForces.insert(it, source);
	}
	else
	{
		it->normal += source.normal;
		it->special += source.special;
	}
}

bool Arrakis::isOccupied(Faction shipper, AreaId where, bool moveAsHostiles) const
{
	if (!isSietch(where))
		return false;

	if (!moveAsHostiles)
		return false;

	static constexpr bool hostility = true;
	static const Faction enemies = Faction::anyExcept(shipper);

	return collectFromSameTerritory(where, &enemies, &hostility).size() >= 2;
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
	static constexpr bool HOSTILITY = true;
	return !collectFromSameArea(AreaId::Carthag, &who, &HOSTILITY).empty() ||
		   !collectFromSameArea(AreaId::Arrakeen, &who, &HOSTILITY).empty();
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

bool
Arrakis::playerForcesInArea(Faction from, AreaId area, ForcesFrom& result) const
{
	auto forces = collectFromSameArea(area, &from, nullptr);

	if (!forces.empty())
	{
		result = *forces.front();
		return true;
	}

	return false;
}

bool
Arrakis::playerForcesInTerritory(Faction from, AreaId childArea, ForcesFrom& result) const
{
	auto forces = collectFromSameTerritory(childArea, &from, nullptr);

	if (!forces.empty())
	{
		result = *forces.front();
		return true;
	}

	return false;
}

std::vector<const ForcesFrom*>
Arrakis::collectFromSameTerritory(AreaId childArea, const Faction* filterFaction, const bool* filterHostile) const
{
	return collect(childArea, filterFaction, filterHostile, false);
}

std::vector<const ForcesFrom*>
Arrakis::collectFromSameArea(AreaId childArea, const Faction* filterFaction, const bool* filterHostile) const
{
	return collect(childArea, filterFaction, filterHostile, true);
}

std::vector<const ForcesFrom*>
Arrakis::collect(AreaId childArea, const Faction* filterFaction, const bool* filterHostile, bool restrictToArea) const
{
	std::vector<const ForcesFrom*> result;

	for (auto& forces : mForces)
	{
		if (!sameTerritory(childArea, forces.where))
			continue;
		if (restrictToArea && childArea != forces.where)
			continue;
		if (filterFaction && !(filterFaction->contains(forces.from)))
			continue;
		if (filterHostile && ((*filterHostile) != forces.hostile))
			continue;

		result.push_back(&forces);
	}

	return result;
}

int Arrakis::hostileFactionsInTerritory(AreaId childArea) const
{
	static constexpr bool HOSTILITY = true;
	return (int) collectFromSameTerritory(childArea, nullptr, &HOSTILITY).size();
}

int Arrakis::hostileEnemiesInTerritory(Faction own, AreaId childArea) const
{
	static constexpr bool HOSTILITY = true;
	const Faction enemies = Faction::anyExcept(own);
	return (int) collectFromSameTerritory(childArea, &enemies, &HOSTILITY).size();
}


int Arrakis::neutralFactionsInTerritory(AreaId childArea) const
{
	static constexpr bool HOSTILITY = false;
	return (int) collectFromSameTerritory(childArea, nullptr, &HOSTILITY).size();
}

std::vector<Conflict> Arrakis::collectConflicts() const
{
	return actualConflicts(potentialConflicts(), storm);
}

std::vector<Conflict> Arrakis::potentialConflicts() const
{
	if (mForces.size() < 2)
		return std::vector<Conflict>();

	std::vector<Conflict> potential;

	Conflict* current = nullptr;
	auto l =   mForces.cbegin();
	auto r = ++mForces.cbegin();

	for (; r != mForces.cend(); ++r)
	{
		if (!sameTerritory(l->where, r->where))
		{
			current = nullptr;
			l = r;
			continue;
		}

		if (!l->hostile)
		{
			++l;
			continue;
		}

		if (!r->hostile)
		{
			continue;
		}

		if (!current)
		{
			potential.emplace_back();
			current = &potential.back();
			current->add(*l);
			current->add(*r);
		}
		else
		{
			current->add(*r);
		}
	}

	return potential;
}

std::vector<Conflict> Arrakis::actualConflicts(const std::vector<Conflict>& potential, int storm) const
{
	std::vector<Conflict> actual;
	for (auto& conflict : potential)
	{
		Conflict::partition(conflict, storm, actual);
	}
	return actual;
}




