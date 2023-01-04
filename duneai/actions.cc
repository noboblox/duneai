#include "actions.h"
#include <iostream>

#include "json.h"

template<>
EnumActionType::Definition EnumActionType::entries =
{
		{ACTION_invalid,                    "INVALID"},
		{ACTION_FACTION_SELECTION,          "factionSelection"},
		{ACTION_PREDICT,                    "predict"},
		{ACTION_HARKONNEN_REDRAW,           "harkonnenRedraw"},
		{ACTION_TRAITOR_SELECTION,          "traitorSelection"},
		{ACTION_FREMEN_PLACEMENT,           "fremenPlacement"},
		{ACTION_BENE_GESSERIT_START_FORCE,  "beneGesseritStartForce"},
		{ACTION_STORM_INITIAL_DIAL,         "stormInitialDial"},
		{ACTION_CHOAM_CHARITY,              "choamCharity"},
		{ACTION_BID,                        "bid"},
		{ACTION_GUILD_SHIPMENT_DECISION,    "guildShipmentDecision"},
		{ACTION_INTRUSION_RESPONSE,         "intrusionResponse"},
		{ACTION_ACCOMPANY_SHIPMENT,         "accompanyShipment"},
		{ACTION_SHIP,                       "ship"},
		{ACTION_MOVE,                       "move"},
		{ACTION_BATTLE_SELECTION,           "battleSelection"},
		{ACTION_COMMIT_BATTLE_PLAN,         "commitBattlePlan"},
		{GM_ACTION_START_GAME,              "startGame"},
};

//static
std::unique_ptr<Action> ActionFactory::create(Faction from, const std::string& json)
{
	const auto type = Action::parseActionType(json);

	switch (type)
	{
	case GM_ACTION_START_GAME:
		return std::make_unique<GmActionStartGame>(from, json);
	case ACTION_FACTION_SELECTION:
		return std::make_unique<ActionFactionSelection>(json);
	case ACTION_PREDICT:
		return std::make_unique<ActionPrediction>(from, json);
//	case ACTION_HARKONNEN_REDRAW:
//		return std::make_unique<ActionHarkonnenRedraw>(from, json);
	case ACTION_TRAITOR_SELECTION:
		return std::make_unique<ActionTraitorSelection>(from, json);
//	case ACTION_FREMEN_PLACEMENT:
//		return std::make_unique<ActionFremenPlacement>(from, json);
//	case ACTION_BENE_GESSERIT_START_FORCE:
//		return std::make_unique<ActionBeneGesseritStartingForce>(from, json);
//	case ACTION_STORM_INITIAL_DIAL:
//		return std::make_unique<ActionStormInitialDial>(from, json);
//	case ACTION_CHOAM_CHARITY:
//		return std::make_unique<ActionChoamCharity>(from, json);
//	case ACTION_BID:
//		return std::make_unique<ActionBid>(from, json);
//	case ACTION_GUILD_SHIPMENT_DECISION:
//		return std::make_unique<ActionGuildShipmentDecision>(from, json);
//	case ACTION_SHIP:
//		return std::make_unique<ActionShip>(from, json);
//	case ACTION_INTRUSION_RESPONSE:
//		return std::make_unique<ActionIntrusionReaction>(from, json);
//	case ACTION_ACCOMPANY_SHIPMENT:
//		return std::make_unique<ActionAccompanyDecision>(from, json);
//	case ACTION_MOVE:
//		return std::make_unique<ActionMove>(from, json);
//	case ACTION_BATTLE_SELECTION:
//		return std::make_unique<ActionBattleSelection>(from, json);
//	case ACTION_COMMIT_BATTLE_PLAN:
//		return std::make_unique<ActionCommitBattlePlan>(from, json);
	default:
		return std::unique_ptr<Action>();
	}
}

static void SuccessOrThrow(bool success)
{
	if (!success)
		throw std::invalid_argument("failed to parse the action correctly");
}

// static
ActionType Action::parseActionType(const std::string& json)
{
    Json doc(json);
    return doc.getActionType("type");
}

// static
Faction ActionFactionSelection::parseFrom(const std::string& json)
{
    Json doc(json);
    return doc.getExactlyOneFaction("from");
}

//-- constructors
ActionFactionSelection::ActionFactionSelection(const std::string& json)
: Action(parseFrom(json), ACTION_FACTION_SELECTION)
{
}

ActionPrediction::ActionPrediction(Faction aFrom, Faction aWinner, int aRound)
: Action(aFrom, ACTION_PREDICT),
  mWinner(aWinner),
  mRound(aRound)
{
}

ActionPrediction::ActionPrediction(Faction aFrom, const std::string& json)
: Action(aFrom, ACTION_PREDICT)
{
	Json doc(json);

	mWinner = doc.getExactlyOneFaction("winner");
	mRound  = doc.getInt("round");
}

ActionTraitorSelection::ActionTraitorSelection(Faction aFrom, Leader::Id aSelection)
: Action(aFrom, ACTION_TRAITOR_SELECTION),
  mSelection(aSelection)
{
}

ActionTraitorSelection::ActionTraitorSelection(Faction aFrom, const std::string& json)
: Action(aFrom, ACTION_TRAITOR_SELECTION)
{
	Json doc(json);
	mSelection = doc.getLeaderId("selection");
}

ActionFremenPlacement::ActionFremenPlacement(Faction aFrom, const std::vector<Placement>& aPlacements)
: Action(aFrom, ACTION_FREMEN_PLACEMENT),
  placements(aPlacements)
{
}

//ActionFremenPlacement::ActionFremenPlacement(Faction aFrom, const std::string& json)
//: Action(aFrom, ACTION_FREMEN_PLACEMENT)
//{
//}

