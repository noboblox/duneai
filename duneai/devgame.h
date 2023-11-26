#pragma once

#include "game.h"

class DevGame : public Game
{
public:
	MACRO_DELETE_ALL_DEFAULTS(DevGame)

	explicit DevGame()
	: Game(GameLogic(true))
	{
	}
};

