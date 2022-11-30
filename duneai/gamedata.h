#ifndef GAMEDATA_H_
#define GAMEDATA_H_

#include "arrakis.h"

struct Placement
{
	Arrakis::AreaId where = Arrakis::PolarSink;
	int normal = 0;
	int special = 0;
};

struct ForcesFrom
{
	Faction from = Faction::none();
	Placement forces;
};

#endif /* GAMEDATA_H_ */
