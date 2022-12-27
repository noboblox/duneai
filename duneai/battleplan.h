#ifndef BATTLEPLAN_H_
#define BATTLEPLAN_H_

#include "gamedata.h"

class BattlePlan
{
public:
	/// create an empty battle plan. This is the default for the own battle plan.
	explicit BattlePlan();

	/// create an unknown battle plan. This is the default for enemy battle plans.
	enum class UnknownPlan { TRUE };
	explicit BattlePlan(UnknownPlan);

	void setOffense(TreacheryCard::Id card);
	void clearOffense(bool unknown = false);
	bool isOffenseKnown() const noexcept;
	TreacheryCard::Id offence() const noexcept;

	void setDefense(TreacheryCard::Id card);
	void clearDefense(bool unknown = false);
	bool isDefenseKnown() const noexcept;

	void setLeader(Leader::Id leader);
	void setCheapHeroLeader(TreacheryCard::Id leader);
	void clearLeader(bool unknown = false);
	bool isLeaderKnown() const noexcept;

	void setForceDial(int dial);
	void clearForceDial(bool unknown = false);
	bool isForceDialKnown() const noexcept;

	void setSpiceDial(int dial);
	void clearSpiceDial(bool unknown = false);
	bool isSpiceDialKnown() const noexcept;

private:
};

#endif /* BATTLEPLAN_H_ */
