#ifndef ACTIONS_H_
#define ACTIONS_H_

#include <vector>
#include <utility>

#include "event.h"

#include "arrakis.h"
#include "enumlabels.h"
#include "gameconstants.h"
#include "gamedata.h"

enum ActionType
{
	ACTION_PREDICT,
	ACTION_HARKONNEN_REDRAW,
	ACTION_TRAITOR_SELECTION,
	ACTION_FREMEN_PLACEMENT,
	ACTION_BENE_GESSERIT_START_FORCE,
	ACTION_STORM_INITIAL_DIAL,
	ACTION_CHOAM_CHARITY,
	ACTION_BID,
	ACTION_GUILD_SHIPMENT_DECISION,
	ACTION_SHIP,
	ACTION_MOVE,
};
using ActionTypeLabels = EnumLabels<ActionType>;

class Action : public Event
{
public:
	static constexpr int VERSION = 1;

	virtual ~Action() {}
	virtual void serialize(std::ostream& out) const;

	Faction from() const noexcept { return mFrom; }
	ActionType type() const noexcept {return mType; }
	const char* label() { return ActionTypeLabels::label(mType); }

protected:
	Action(Faction aFrom, ActionType aType)
	: Event(ET_ACTION),
	  mFrom(aFrom), mType(aType)
	{
	}

private:
	Faction mFrom;
	ActionType mType;
};

class ActionPrediction : public Action
{
public:
	explicit ActionPrediction(Faction aFrom, Faction aWinner, int aRound)
	: Action(aFrom, ACTION_PREDICT),
	  winner(aWinner),
	  round(aRound)
	{
	}

	virtual void serialize(std::ostream& out) const override;

	const Faction winner;
	const int round;
};

class ActionTraitorSelection : public Action
{
public:
	explicit ActionTraitorSelection(Faction aFrom, Leader::Id aSelection)
	: Action(aFrom, ACTION_TRAITOR_SELECTION),
	  selection(aSelection)
	{
	}

	virtual void serialize(std::ostream& out) const override;
	const Leader::Id selection;
};

class ActionFremenPlacement : public Action
{
public:
	explicit ActionFremenPlacement(Faction aFrom, const std::vector<Placement>& aPlacements)
	: Action(aFrom, ACTION_FREMEN_PLACEMENT),
	  placements(aPlacements)
	{
	}

	virtual void serialize(std::ostream& out) const override;
	const std::vector<Placement> placements;
};

class ActionHarkonnenRedraw : public Action
{
public:
	explicit ActionHarkonnenRedraw(Faction aFrom, bool aValue)
	: Action(aFrom, ACTION_HARKONNEN_REDRAW),
	  redraw(aValue)
	{
	}

	virtual void serialize(std::ostream& out) const override;
	const bool redraw;
};

class ActionBeneGesseritStartingForce : public Action
{
public:
	explicit ActionBeneGesseritStartingForce(Faction aFrom, AreaId aWhere)
	: Action(aFrom, ACTION_BENE_GESSERIT_START_FORCE),
	  where(aWhere)
	{
	}

	virtual void serialize(std::ostream& out) const override;
	const AreaId where;
};

class ActionStormInitialDial : public Action
{
public:
	explicit ActionStormInitialDial(Faction aFrom, int aDial)
	: Action(aFrom, ACTION_STORM_INITIAL_DIAL),
	  dial(aDial)
	{
	}

	virtual void serialize(std::ostream& out) const override;
	const int dial;
};

class ActionChoamCharity : public Action
{
public:
	explicit ActionChoamCharity(Faction aFrom, bool aNeed)
	: Action(aFrom, ACTION_CHOAM_CHARITY),
	  need(aNeed)
	{
	}

	virtual void serialize(std::ostream& out) const override;
	const bool need;
};

class ActionBid : public Action
{
public:
	enum Type
	{
		RAISE,
		PASS,
		KARAMA_BUY
	};

	explicit ActionBid(Faction aFrom, int aBid)
	: Action(aFrom, ACTION_BID),
	  type(RAISE),
	  bid(aBid)
	{
	}

	explicit ActionBid(Faction aFrom, Type aType)
	: Action(aFrom, ACTION_BID),
	  type(aType),
	  bid(0)
	{
	}

	virtual void serialize(std::ostream& out) const override;
	const Type type;
	const int bid;
};

class ActionGuildShipmentDecision : public Action
{
public:
	explicit ActionGuildShipmentDecision(Faction aFrom, bool aShipNow)
	: Action(aFrom, ACTION_GUILD_SHIPMENT_DECISION),
	  shipNow(aShipNow)
	{
	}

	virtual void serialize(std::ostream& out) const override;
	const bool shipNow;
};

class ActionShip : public Action
{
public:
	enum InvertShipment { TRUE };

	/**
	 * perform a standard shipment.
	 * forces are taken from reserve
	 * @note this event is also applicable for fremen shipments
	 */
	explicit ActionShip(Faction aFaction, Placement aTo)
	: Action(aFaction, ACTION_SHIP),
	  to(aTo), fromArea(AreaId::INVALID), fromReserve(true), inverted(false)
	{
	}

	/**
	 * perform an on-planet shipment.
	 * Usually this is allowed for the guild player only
	 */
	explicit ActionShip(Faction aFaction, AreaId aFrom, Placement aTo)
	: Action(aFaction, ACTION_SHIP),
	  to(aTo), fromArea(aFrom), fromReserve(false), inverted(false)
	{
	}

	/**
	 * perform a very special FROM planet TO reserve shipment.
	 * Usually this is allowed for the guild player only
	 */
	explicit ActionShip(Faction aFaction, Placement aFrom, InvertShipment)
	: Action(aFaction, ACTION_SHIP),
	  to(aFrom), fromArea(AreaId::INVALID), fromReserve(true), inverted(true)
	{
	}

	virtual void serialize(std::ostream& out) const override;

	const Placement to;
	const AreaId fromArea;
	const bool fromReserve;
	const bool inverted;
};

class ActionMove : public Action
{
	explicit ActionMove(Faction aFaction, AreaId aFrom, Placement aTo, bool aUseHajr = false)
	: Action(aFaction, ACTION_MOVE),
	  to(aTo), fromArea(aFrom), useHajr(aUseHajr)
	{
	}

	virtual void serialize(std::ostream& out) const override;

	const Placement to;
	const AreaId fromArea;
	const bool useHajr;
};


#endif /* ACTIONS_H_ */
