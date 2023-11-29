#include "actions.h"

#include <iostream>

template<>
EnumActionType::Definition EnumActionType::entries =
{
		{ACTION_invalid,                    "INVALID"},
		{ACTION_FACTION_SELECTION,          "factionSelection"},
		{GM_ACTION_START_GAME,              "startGame"},
		{ACTION_PREDICT,                    "predict"},
		{ACTION_HARKONNEN_REDRAW,           "harkonnenRedraw"},
		{ACTION_TRAITOR_SELECTION,          "traitorSelection"},
		{ACTION_FREMEN_PLACEMENT,           "fremenPlacement"},
		{ACTION_BENE_GESSERIT_START_FORCE,  "beneGesseritStartForce"},
		{ACTION_STORM_DIAL,                 "stormDial"},
		{ACTION_CHOAM_CHARITY,              "choamCharity"},
		{ACTION_BID,                        "bid"},
		{ACTION_GUILD_SHIPMENT_DECISION,    "guildShipmentDecision"},
		{ACTION_INTRUSION_RESPONSE,         "intrusionResponse"},
		{ACTION_ACCOMPANY_SHIPMENT,         "accompanyShipment"},
		{ACTION_SHIP,                       "ship"},
		{ACTION_MOVE,                       "move"},
		{ACTION_BATTLE_SELECTION,           "battleSelection"},
		{ACTION_COMMIT_BATTLE_PLAN,         "commitBattlePlan"},
		{DEV_ACTION_DIAL_TREACHERY_CARD,    "Dev.dialTreacheryCard"},
		{DEV_ACTION_PLACE_TROOPS,           "Dev.placeTroops"},
		{DEV_ACTION_SET_STORM,              "Dev.setStorm"},
		{DEV_ACTION_SET_GAME_PHASE,         "Dev.setGamePhase"},
};

static void SuccessOrThrow(bool success)
{
	if (!success)
		throw std::invalid_argument("failed to parse the action correctly");
}

ActionPrediction::ActionPrediction(Faction aFrom, Faction aWinner, int aRound)
: Action(aFrom, ACTION_PREDICT),
  mWinner(aWinner),
  mRound(aRound)
{
}

ActionTraitorSelection::ActionTraitorSelection(Faction aFrom, Leader::Id aSelection)
: Action(aFrom, ACTION_TRAITOR_SELECTION),
  mSelection(aSelection)
{
}

