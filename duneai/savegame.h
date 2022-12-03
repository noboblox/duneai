#ifndef SAVEGAME_H_
#define SAVEGAME_H_

#include "gamedata.h"
#include "actions.h"
#include <memory>

class SaveGame
{
public:
	SaveGame(const GameState& state, const std::vector<std::unique_ptr<const Action>>& actions);
	const std::string& getData() const noexcept { return mData; }

private:
	std::string mData;
};

#endif /* SAVEGAME_H_ */
