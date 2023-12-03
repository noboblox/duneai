#ifndef TERRITORIES_H_
#define TERRITORIES_H_

#include <vector>
#include "partialterritory.h"

class Territories
{
public:
	static const Territory& of(AreaId area);

	static const Territory polarSink;
	static const Territory falseWallEast;
	static const Territory theMinorErg;
	static const Territory pastyMesa;
	static const Territory redChasm;
	static const Territory shieldWall;
	static const Territory sihayaRidge;
	static const Territory holeInTheRock;
	static const Territory basin;
	static const Territory rimWallWest;
	static const Territory imperialBasin;
	static const Territory garaKulon;
	static const Territory arrakeen;
	static const Territory oldGap;
	static const Territory carthag;
	static const Territory tsimpo;
	static const Territory brokenLand;
	static const Territory arsunt;
	static const Territory plasticBasin;
	static const Territory haggaBasin;
	static const Territory rockOutcroppings;
	static const Territory windPass;
	static const Territory sietchTabr;
	static const Territory bightOfTheCliff;
	static const Territory funeralPlain;
	static const Territory theGreatFlat;
	static const Territory theGreaterFlat;
	static const Territory habbanyaErg;
	static const Territory falseWallWest;
	static const Territory habbanyaRidgeFlat;
	static const Territory cielagoWest;
	static const Territory windPassNorth;
	static const Territory cielagoNorth;
	static const Territory hargPass;
	static const Territory habbanyaSietch;
	static const Territory falseWallSouth;
	static const Territory meridian;
	static const Territory cielagoDepression;
	static const Territory cielagoSouth;
	static const Territory cielagoEast;
	static const Territory southMesa;
	static const Territory tueksSietch;

private:
	struct TerritoryForArea
	{
		AreaId child;
		const Territory* parent;
	};

	static const std::vector<TerritoryForArea> msAreaToTerritory;
};

#endif /* TERRITORIES_H_ */
