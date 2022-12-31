#ifndef BATTLEPLAN_H_
#define BATTLEPLAN_H_

#include "gamedata.h"

class BattlePlan
{
public:
	struct Data
	{
		TreacheryCard::Id offense   = TreacheryCard::INVALID;
		TreacheryCard::Id defense   = TreacheryCard::INVALID;
		TreacheryCard::Id cheapHero = TreacheryCard::INVALID;
		Leader::Id        hero      = Leader::ID_INVALID;
		int               forceDial = 0;
		int               spiceDial = 0;
	};

public:
	/// create an empty battle plan. This is the default for the own battle plan.
	explicit BattlePlan() : data(), known(ALL_KNOWN) {}
	
	/// create a known battle plan from exising data.
	explicit BattlePlan(const BattlePlan::Data& source) : data(source), known(ALL_KNOWN) {}

	/// create an unknown battle plan. This is the default for the enemy battle plan.
	enum class UnknownPlan { TRUE };
	explicit BattlePlan(UnknownPlan) : data(), known(NOTHING_KNOWN) {}

	void setOffense(TreacheryCard::Id card) noexcept;
	void clearOffense(bool unknown = false) noexcept;
	bool isOffenseKnown() const noexcept;
	TreacheryCard::Id offence() const noexcept;

	void setDefense(TreacheryCard::Id card) noexcept;
	void clearDefense(bool unknown = false) noexcept;
	bool isDefenseKnown() const noexcept;
	TreacheryCard::Id defense() const noexcept;

	void setLeader(Leader::Id leader) noexcept;
	void setCheapHeroLeader(TreacheryCard::Id leader) noexcept;

	void clearLeader(bool unknown = false) noexcept;
	bool isLeaderKnown() const noexcept;
	std::pair<TreacheryCard::Id, Leader::Id> leader() const noexcept;

	void setForceDial(int dial) noexcept;
	void clearForceDial(bool unknown = false) noexcept;
	bool isForceDialKnown() const noexcept;
	int forceDial() const noexcept;

	void setSpiceDial(int dial) noexcept;
	void clearSpiceDial(bool unknown = false) noexcept;
	bool isSpiceDialKnown() const noexcept;
	int spiceDial() const noexcept;

	Data copyContent() const noexcept { return data; }

	bool validate(const PlayerState& player, AreaId area) const noexcept;

private:
	enum Known
	{
		KNOWN_OFFENSE    = 0x01,
		KNOWN_DEFENSE    = 0x02,
		KNOWN_HERO       = 0x04,
		KNOWN_FORCE_DIAL = 0x08,
		KNOWN_SPICE_DIAL = 0x10,

		ALL_KNOWN        = 0x1F,
		NOTHING_KNOWN    = 0x00,
	};

	void setKnown(Known mask, bool value) noexcept;

	Data data;
	int known;
};

#endif /* BATTLEPLAN_H_ */
