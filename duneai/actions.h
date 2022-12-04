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
	ACTION_STORM_INITIAL_DIAL
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

#endif /* ACTIONS_H_ */
