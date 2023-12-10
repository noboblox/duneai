#ifndef BATTLE_H_
#define BATTLE_H_

#include "conflict.h"

class Battle
{
public:
	explicit Battle() noexcept;
	explicit Battle(Conflict&& c);

private:
	Conflict mConflict;
};

#endif /* BATTLE_H_ */
