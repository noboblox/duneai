#ifndef ACTIONS_H_
#define ACTIONS_H_

#include <vector>
#include <utility>

#include "enumlabels.h"
#include "gameconstants.h"

enum ActionType
{
	ACTION_PREDICT,
	ACTION_TRAITOR_SELECTION,
	ACTION_FREMEN_PLACEMENT,
};
using ActionTypeLabels = EnumLabels<ActionType>;


class Action
{
public:

	virtual ~Action() {}

	Faction from() const noexcept { return mFrom; }
	ActionType type() const noexcept {return mType; }
	const char* label() { return ActionTypeLabels::label(mType); }

protected:
	Action(Faction aFrom, ActionType aType)
	: mFrom(aFrom), mType(aType)
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

	const Leader::Id selection;
};

class ActionFremenPlacement : public Action
{
public:
	using Placement = std::vector<std::pair<int, int>>;

	explicit ActionFremenPlacement(Faction aFrom, const Placement& aPlacement)
	: Action(aFrom, ACTION_FREMEN_PLACEMENT),
	  placement(aPlacement)
	{
	}

	const Placement placement;
};

#endif /* ACTIONS_H_ */
