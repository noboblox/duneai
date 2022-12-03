/*
 * DebugWindow.h
 *
 *  Created on: 03.12.2022
 *      Author: Rene
 */

#ifndef TOOLS_DEBUGWINDOW_H_
#define TOOLS_DEBUGWINDOW_H_

#include <memory>
#include <vector>
#include "../gamelogic.h"

class DebugWindow
{
public:
	explicit DebugWindow();
	~DebugWindow();

	void updateWindow(bool& showImguiDemo);

private:
	std::unique_ptr<GameLogic> mpEngine;
	std::vector<std::pair<Faction, bool>> factionEnabled;
	std::vector<std::string> mLog;

	Faction getSendFaction() const;
	Faction getFactionFrom(const std::vector<std::pair<Faction, bool>>& factions) const;

	void controlWindow(bool& showImguiDemo);
	void gameStateWindow(const GameState& state);
	void logWindow();
	void addLogEntry(const char* msg);

	bool uniqueFaction = true;
	int uniqueFactionIndex = 0;
};

#endif /* TOOLS_DEBUGWINDOW_H_ */
