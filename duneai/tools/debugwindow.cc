#include "debugwindow.h"
#include "../gamelogic.h"

#include "../imgui/imgui.h"

static void InitOnce(std::vector<std::pair<Faction, bool>>& result);
static void DrawFactions(std::vector<std::pair<Faction, bool>>& factions,
		                 const bool* unique = nullptr, int* uniqueIndex = nullptr);


DebugWindow::DebugWindow()
: mpEngine(new GameLogic(Faction::any()))
{
	InitOnce(factionEnabled);
}

DebugWindow::~DebugWindow()
{
}

void DebugWindow::updateWindow(bool& showImguiDemo)
{
	ImGui::Begin("DUNE logic debug tool");
	ImGui::Checkbox("show imgui demo", &showImguiDemo);


	ImGui::Text("Faction for action"); ImGui::SameLine();
	ImGui::Checkbox("unique", &uniqueFaction);

	DrawFactions(factionEnabled, &uniqueFaction, &uniqueFactionIndex);

	if (ImGui::CollapsingHeader("Init Actions"))
	{
        if (ImGui::TreeNode("Predict"))
        {
        	static std::vector<std::pair<Faction, bool>> predicted;
        	InitOnce(predicted);
        	DrawFactions(predicted);
        	ImGui::TreePop();
        }
        if (ImGui::TreeNode("Harkonnen mulligan"))
        {
        	ImGui::TreePop();
        }
        if (ImGui::TreeNode("Traitor selection"))
        {
        	ImGui::TreePop();
        }
        if (ImGui::TreeNode("Fremen placement"))
        {
        	ImGui::TreePop();
        }
        if (ImGui::TreeNode("Bene Gesserit starting position"))
        {
        	ImGui::TreePop();
        }
	}

	mpEngine->tick();
	ImGui::End();
}

void InitOnce(std::vector<std::pair<Faction, bool>>& result)
{
	if (!result.empty())
		return;

	std::vector<Faction> factions = Faction::expand(Faction::any());

	for (const auto& f : factions)
	{
		result.emplace_back(f, false);
	}
}

void DrawFactions(std::vector<std::pair<Faction, bool>>& factions,
		          const bool* unique, int* uniqueIndex)
{
	for (int i = 0; i < (int) factions.size(); ++i)
	{
		auto& f = factions[i];
		if (unique && (*unique == true))
			ImGui::RadioButton(f.first.label().c_str(), uniqueIndex, i);
		else
			ImGui::Checkbox(f.first.label().c_str(), &(f.second));

		if (((i + 1) % 4) && (i != (int) factions.size() - 1))
			ImGui::SameLine();
	}
}
