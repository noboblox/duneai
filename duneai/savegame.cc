#include "savegame.h"
#include <sstream>

SaveGame::SaveGame(const GameState& state, const std::vector<std::unique_ptr<const Action>>& actions)
{
	std::ostringstream out;

	out << GameState::VERSION << Action::VERSION
		<< state.maxRound << state.seed;

	Faction factionsInGame = Faction::none();
	for (const auto& p : state.players)
	{
		factionsInGame = factionsInGame | p.faction;
	}

	for (const auto& ac : actions)
	{
		ac->serialize(out);
	}

	mData.assign(out.str());
}
