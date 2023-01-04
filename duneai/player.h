#ifndef PLAYER_H_
#define PLAYER_H_

#include "actions.h"
#include "gamelogic.h"

class Player
{
public:
	using Target = std::function<void(std::unique_ptr<Action>&&)>;

	Player(Faction own, Target target, bool gameMaster = false);

	void setTarget(Target newTarget) { invokeTarget = newTarget; }
	Faction faction() { return own; }

private:
	Faction own;
	Target invokeTarget;
	bool gameMaster;
};

#endif /* PLAYER_H_ */
