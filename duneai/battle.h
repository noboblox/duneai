#ifndef BATTLE_H_
#define BATTLE_H_

#include "gameconstants.h"
#include "partialterritory.h"

class GameState;

class Battle {
public:
	explicit Battle();
	Battle(const GameState& state, Faction aggressor, Faction defender, PartialTerritory battleArea);
};

#endif /* BATTLE_H_ */
