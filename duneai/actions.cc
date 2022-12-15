#include "actions.h"
#include <iostream>

template<>
std::unordered_map<ActionType, const char*> ActionTypeLabels::labels =
{
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
		{ACTION_ACOMMPANY_SHIPMENT,         "accompanyShipment"},
		{ACTION_SHIP,                       "ship"},
		{ACTION_MOVE,                       "move"},
};

void Action::serialize(std::ostream& out) const
{
	out << static_cast<int> (mType)
		<< static_cast<int> (mFrom);
}

void ActionFremenPlacement::serialize(std::ostream& out) const
{
	Action::serialize(out);

	out << static_cast<int> (placements.size());
	for (const auto& p : placements)
	{
		out << static_cast<int> (p.where)
		    << p.normal
			<< p.special;
	}
}

void ActionBeneGesseritStartingForce::serialize(std::ostream& out) const
{
	Action::serialize(out);
	out << static_cast<int> (where);
}

void ActionHarkonnenRedraw::serialize(std::ostream& out) const
{
	Action::serialize(out);
	out << static_cast<int> (redraw);
}

void ActionPrediction::serialize(std::ostream& out) const
{
	Action::serialize(out);
	out << static_cast<int> (winner)
		<< round;
}

void ActionTraitorSelection::serialize(std::ostream& out) const
{
	Action::serialize(out);
	out << static_cast<int> (selection);
}

void ActionStormInitialDial::serialize(std::ostream& out) const
{
	Action::serialize(out);
	out << dial;
}

void ActionChoamCharity::serialize(std::ostream& out) const
{
	Action::serialize(out);
	out << static_cast<int> (need);
}

void ActionBid::serialize(std::ostream& out) const
{
	Action::serialize(out);
	out << static_cast<int> (type)
		<< bid;
}

void ActionGuildShipmentDecision::serialize(std::ostream& out) const
{
	Action::serialize(out);
	out << static_cast<int> (shipNow);
}

void ActionShip::serialize(std::ostream& out) const
{
	Action::serialize(out);
	out << static_cast<int> (to.where)
		<< to.normal << to.special
		<< static_cast<int> (fromArea)
		<< static_cast<int> (fromReserve)
		<< static_cast<int> (inverted);
}

void ActionIntrusionReaction::serialize(std::ostream& out) const
{
	Action::serialize(out);
	out << static_cast<int> (disengage);
}

void ActionAccompanyDecision::serialize(std::ostream& out) const
{
	Action::serialize(out);
	out << static_cast<int> (decision);
}

void ActionMove::serialize(std::ostream& out) const
{
	Action::serialize(out);
	out << static_cast<int> (to.where)
		<< to.normal << to.special
		<< static_cast<int> (fromArea)
		<< static_cast<int> (useHajr);
}
