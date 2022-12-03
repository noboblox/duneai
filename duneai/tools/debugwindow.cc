#include "debugwindow.h"
#include "../gamelogic.h"

#include "../imgui/imgui.h"
#include "imguilogger.h"

static void InitOnce(std::vector<std::pair<Faction, bool>>& result);
static void FactionList(std::vector<std::pair<Faction, bool>>& factions,
		                 const bool* unique = nullptr, int* uniqueIndex = nullptr);

static bool SendButton(ActionType id);

DebugWindow::DebugWindow()
: mpEngine(new GameLogic())
{
	mpEngine->setLogger(std::make_unique<ImGuiLogger>([this](const char* msg){ this->addLogEntry(msg); }));
	mpEngine->setup(Faction::any());
	InitOnce(factionEnabled);
}

DebugWindow::~DebugWindow()
{
}

void DebugWindow::updateWindow(bool& showImguiDemo)
{
	controlWindow(showImguiDemo);
	logWindow();
}

void DebugWindow::controlWindow(bool& showImguiDemo)
{
	ImGui::Begin("DUNE logic debug tool");
	ImGui::Checkbox("show imgui demo", &showImguiDemo);


	ImGui::Text("Faction for action"); ImGui::SameLine();
	ImGui::Checkbox("unique", &uniqueFaction);

	FactionList(factionEnabled, &uniqueFaction, &uniqueFactionIndex);

	if (ImGui::CollapsingHeader("Init Actions"))
	{
        if (ImGui::TreeNode("Predict"))
        {
        	static std::vector<std::pair<Faction, bool>> predicted;
        	static int round = 1;

        	InitOnce(predicted);
        	FactionList(predicted);
        	ImGui::SliderInt("round", &round, -1, 21);

        	if (SendButton(ACTION_PREDICT))
        	{
        		mpEngine->post(std::make_unique<ActionPrediction>(getSendFaction(),
        				getFactionFrom(predicted), round));
        	}

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

	mpEngine->requestGameState([this](const GameState& gs){ this->gameStateWindow(gs); });
	mpEngine->tick();
	ImGui::End();
}

void DebugWindow::logWindow()
{
	ImGui::Begin("log console");

	for (const auto& msg : mLog)
	{
		ImGui::Text(msg.c_str());
	}

	ImGui::End();
}


static void fillRow(const char* name, const char* fmt, ...)
{
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::Text(name);
	ImGui::TableSetColumnIndex(1);
    va_list args;
    va_start(args, fmt);
    ImGui::TextV(fmt, args);
    va_end(args);
}

void DebugWindow::gameStateWindow(const GameState& state)
{
	ImGui::Begin("game state");

    if (ImGui::BeginTable("Game State", 2))
    {
        ImGui::TableSetupColumn("name", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("value", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();

        fillRow("seed",                "%u", state.seed);
        fillRow("round",               "%d", state.round);
        fillRow("maxRound",            "%d", state.maxRound);
        fillRow("phase",               "%s", GamePhaseLabels::label(state.phase));
        fillRow("input expected from", "%s", state.expectingInputFrom.label().c_str());
        fillRow("predicted faction",   "%s", state.predictedFaction.label().c_str());
        fillRow("predicted turn",      "%d", state.predictedTurn);
        ImGui::EndTable();
    }

	ImGui::End();
}

Faction DebugWindow::getFactionFrom(const std::vector<std::pair<Faction, bool>>& factions) const
{
		Faction result = Faction::none();
		for (const auto& f : factions)
		{
			if (f.second)
				result = result | f.first;
		}
		return result;
}

Faction DebugWindow::getSendFaction() const
{
	if (uniqueFaction)
		return factionEnabled[uniqueFactionIndex].first;
	else
		return getFactionFrom(factionEnabled);
}

void DebugWindow::addLogEntry(const char* msg)
{
	mLog.emplace_back(msg);
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

void FactionList(std::vector<std::pair<Faction, bool>>& factions,
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

bool SendButton(ActionType id)
{
    ImGui::PushID(id);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f));
    const bool result = ImGui::Button("Send");
    ImGui::PopStyleColor(3);
    ImGui::PopID();

    return result;
}

