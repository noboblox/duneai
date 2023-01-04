#pragma once

#include <memory>
#include "gamelogic.h"
#include "player.h"

class Game
{
public:
    explicit Game() {}
    std::shared_ptr<Player> createPlayer();

private:
    std::vector<std::shared_ptr<Player>> players;
    GameLogic logic;
};
