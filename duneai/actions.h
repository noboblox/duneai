#ifndef ACTIONS_H_
#define ACTIONS_H_

#include <vector>
#include <utility>

#include "arrakis.h"
#include "enumlabels.h"
#include "gameconstants.h"
#include "gamedata.h"

enum ActionType
{
	ACTION_PREDICT,
	ACTION_TRAITOR_SELECTION,
	ACTION_FREMEN_PLACEMENT,
	ACTION_BENE_GESSERIT_PLACEMENT,
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
	explicit ActionFremenPlacement(Faction aFrom, const std::vector<Placement>& aPlacements)
	: Action(aFrom, ACTION_FREMEN_PLACEMENT),
	  placements(aPlacements)
	{
	}

	const std::vector<Placement> placements;
};

#endif /* ACTIONS_H_ */
