#ifndef ARRAKIS_H_
#define ARRAKIS_H_

#include <vector>

class Arrakis
{
public:
	enum AreaId
	{
		PolarSink              = 10 ,
		FalseWallEast_5        = 20 ,
		FalseWallEast_6        = 21 ,
		FalseWallEast_7        = 22 ,
		FalseWallEast_8        = 23 ,
		FalseWallEast_9        = 24 ,
		TheMinorErg_5          = 30 ,
		TheMinorErg_6          = 31 ,
		TheMinorErg_7          = 32 ,
		TheMinorErg_8          = 33 ,
		PastyMesa_5            = 40 ,
		PastyMesa_6            = 41 ,
		PastyMesa_7            = 42 ,
		PastyMesa_8            = 43 ,
		RedChasm               = 50 ,
		ShieldWall_8           = 60 ,
		ShieldWall_9           = 61 ,
		SihayaRidge            = 70 ,
		HoleInTheRock          = 80 ,
		Basin                  = 90 ,
		RimWallWest            = 100,
		ImperialBasin_9        = 110,
		ImperialBasin_10       = 111,
		ImperialBasin_11       = 112,
		GaraKulon              = 120,
		Arrakeen               = 130,
		OldGap_9               = 140,
		OldGap_10              = 141,
		OldGap_11              = 142,
		Carthag                = 150,
		Tsimpo_11              = 160,
		Tsimpo_12              = 161,
		Tsimpo_13              = 162,
		BrokenLand_11          = 170,
		BrokenLand_12          = 171,
		Arsunt_11              = 180,
		Arsunt_12              = 181,
		PlasticBasin_12        = 190,
		PlasticBasin_13        = 191,
		PlasticBasin_14        = 192,
		HaggaBasin_12          = 200,
		HaggaBasin_13          = 201,
		RockOutcroppings_13    = 210,
		RockOutcroppings_14    = 211,
		WindPass_14            = 220,
		WindPass_15            = 221,
		WindPass_16            = 222,
		WindPass_17            = 223,
		SietchTabr             = 230,
		BightOfTheCliff_14     = 240,
		BightOfTheCliff_15     = 241,
		FuneralPlain           = 250,
		TheGreatFlat           = 260,
		TheGreaterFlat         = 270,
		HabbanyaErg_16         = 280,
		HabbanyaErg_17         = 281,
		FalseWallWest_16       = 290,
		FalseWallWest_17       = 291,
		FalseWallWest_18       = 292,
		HabbanyaRidgeFlat_17   = 300,
		HabbanyaRidgeFlat_18   = 301,
		CielagoWest_18         = 310,
		CielagoWest_1          = 311,
		WindPassNorth_17       = 320,
		WindPassNorth_18       = 321,
		CielagoNorth_1         = 330,
		CielagoNorth_2         = 331,
		CielagoNorth_3         = 332,
		HargPass_4             = 340,
		HargPass_5             = 341,
		HabbanyaSietch         = 350,
		FalseWallSouth_4       = 360,
		FalseWallSouth_5       = 361,
		Meridian_1             = 370,
		Meridian_2             = 371,
		CielagoDepression_1    = 380,
		CielagoDepression_2    = 381,
		CielagoDepression_3    = 382,
		CielagoSouth_2         = 390,
		CielagoSouth_3         = 391,
		CielagoEast_3          = 400,
		CielagoEast_4          = 401,
		SouthMesa_4            = 410,
		SouthMesa_5            = 411,
		SouthMesa_6            = 412,
		TueksSietch            = 420,
	};


    static void reachable(AreaId from, int storm, int movement, std::vector<AreaId>& result);
    static bool insideStorm(AreaId id, int storm);

    static const char* areaName(AreaId id);
private:
	Arrakis();

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
