#ifndef GAMECONSTANTS_H_
#define GAMECONSTANTS_H_

#include <array>
#include <string>
#include <vector>
#include "enumlabels.h"
/// enum containing all stages of the game
enum GamePhase
{
	PHASE_invalid    = 0,

	PHASE_INIT_JOIN = 1,
	PHASE_INIT_PREDICTION,
	PHASE_INIT_HARKONNEN_REDRAW,
	PHASE_INIT_TRAITOR_SELECTION,
	PHASE_INIT_FREMEN_PLACEMENT,
	PHASE_INIT_BG_PLACEMENT,

	PHASE_STORM_INITAL_DIAL,

	// TODO spice blow is currently done automatically
	PHASE_SPICE_SPICE_BLOW,

	PHASE_CHOAM_CHARITY,
	PHASE_AUCTION_BIDDING,
	PHASE_SHIPMENT_GUILD_DECISION,
	PHASE_SHIPMENT_SHIP,
	PHASE_SHIPMENT_INTRUSION_REACTION,
	PHASE_SHIPMENT_ACCOMPANY_DECISION,
	PHASE_SHIPMENT_MOVE,
	PHASE_BATTLE_COLLECT_BATTLES,
	PHASE_BATTLE_SELECTION,
	PHASE_BATTLE_BEGIN,
	PHASE_SPICE_HARVEST,

};
using GamePhaseLabels = Enum<GamePhase, PHASE_invalid>;

enum BiddingAction
{
	BID_ACTION_invalid = -1,

	PASS = 0,
	BID,
	KARAMA_BID,
	KARAMA_BUY,
};
using EnumBiddingAction = Enum<BiddingAction, BID_ACTION_invalid>;

class SeatConfig
{
public:
	static const std::vector<int>& getConfig(int players)
	{
		if (players < 2 || players > 10)
			return INVALID;
		else
			return *(configs[players]);
	}

private:
	static const std::vector<int> INVALID     ;
	static const std::vector<int> twoPlayers  ;
	static const std::vector<int> threePlayers;
	static const std::vector<int> fourPlayers ;
	static const std::vector<int> fivePlayers ;
	static const std::vector<int> sixPlayers  ;
	static const std::vector<int> sevenPlayers;
	static const std::vector<int> eightPlayers;
	static const std::vector<int> ninePlayers ;
	static const std::vector<int> tenPlayers  ;

	static const std::vector<int>* const configs[];
};

class Faction
{
public:

	static constexpr Faction emperor()                   { return Faction(CODE_EMPEROR); }
	static constexpr Faction spacingGuild()              { return Faction(CODE_SPACING_GUILD); }
	static constexpr Faction fremen()                    { return Faction(CODE_FREMEN); }
	static constexpr Faction atreides()                  { return Faction(CODE_ATREIDES); }
	static constexpr Faction harkonnen()                 { return Faction(CODE_HARKONNEN); }
	static constexpr Faction beneGesserit()              { return Faction(CODE_BENE_GESSERIT); }
	static constexpr Faction tleilaxu()                  { return Faction(CODE_TLEILAXU); }

	static constexpr Faction none()                      { return Faction(CODE_NONE); }
	static constexpr Faction any()                       { return Faction(CODE_ANY); }
	static constexpr Faction anyExcept(Faction excluded) { return Faction(any().mValue & (~excluded.mValue)); }
	static std::vector<Faction> expand(Faction mask);
	static Faction fromString(const std::string& label);

	explicit operator int() const noexcept
    {
		return mValue;
    }


	std::string label() const
	{
		std::string o = codeLabel(mValue);

		if (o != "UNDEFINED")
			return o;

		if (mValue > CODE_ANY)
			return std::string("CORRUPT");

		o.clear();
		o.push_back('[');

		for (int flag = 0x01; flag < CODE_ANY; flag <<= 1)
		{
			if (flag & mValue)
			{
				o.append(codeLabel(flag));
				o.push_back(',');
			}
		}

		o.pop_back();
		o.push_back(']');
		return o;
	}

	constexpr Faction(const Faction& other)
	: mValue(other.mValue)
	{
	}

	constexpr explicit Faction():  mValue(CODE_NONE) {}
	constexpr explicit Faction(int value):  mValue(value) {}

	bool operator==(Faction other) const noexcept { return mValue == other.mValue; }
	bool operator!=(Faction other) const noexcept { return !operator==(other); }
	bool operator<(Faction other) const noexcept { return mValue < other.mValue; }

	Faction operator&(Faction other)
	{
		return Faction(mValue & other.mValue);
	}

	Faction operator|(Faction other)
	{
		return Faction(mValue | other.mValue);
	}

	Faction& operator&=(Faction other)
	{
		mValue &= other.mValue;
		return *this;
	}

	Faction& operator|=(Faction other)
	{
		mValue |= other.mValue;
		return *this;
	}

	void set(Faction added) noexcept
	{
		mValue |= added.mValue;
	}

	void clear(Faction removed) noexcept
	{
		mValue &= (~removed.mValue);
	}

	constexpr bool contains(Faction faction) const noexcept
	{
		return faction.mValue & mValue;
	}

	bool exactlyOne() const noexcept { return count() == 1; }

	int count() const noexcept
	{
		int count = 0;
		for (int i = 1; i <= CODE_ANY; i <<= 1)
		{
			if (mValue & i)
				++count;
		}
		return count;
	}

	// number of all factions
	static constexpr int COUNT = 7;

	// array of all factions in ascending order
	static const std::array<Faction, Faction::COUNT>& allFactions();

private:
	static constexpr int CODE_NONE          = 0;

	static constexpr int CODE_EMPEROR       = 1 << 0;
	static constexpr int CODE_SPACING_GUILD = 1 << 1;
	static constexpr int CODE_FREMEN        = 1 << 2;
	static constexpr int CODE_ATREIDES      = 1 << 3;
	static constexpr int CODE_HARKONNEN     = 1 << 4;
	static constexpr int CODE_BENE_GESSERIT = 1 << 5;
	static constexpr int CODE_TLEILAXU      = 1 << 6;

	static constexpr int CODE_ANY           = (1 << COUNT) - 1;


	static const char* codeLabel(int value)
	{
		switch (value)
		{
		case CODE_ANY          : return "any";
		case CODE_NONE         : return "none";
		case CODE_EMPEROR      : return "emperor";
		case CODE_SPACING_GUILD: return "spacingGuild";
		case CODE_FREMEN       : return "fremen";
		case CODE_ATREIDES     : return "atreides";
		case CODE_HARKONNEN    : return "harkonnen";
		case CODE_BENE_GESSERIT: return "beneGesserit";
		case CODE_TLEILAXU     : return "tleilaxu";
		default:                 return "UNDEFINED";
		}
	}

	int mValue;
};

class Leader
{
public:
	enum Id
	{
		ID_INVALID    = -1,
		LEADERS_begin =  0,

		ID_Yueh   = LEADERS_begin, //ATREIDES
		ID_Duncan , //ATREIDES
		ID_Gurney , //ATREIDES
		ID_Thufir , //ATREIDES
		ID_Jessica, //ATREIDES

		ID_Alia   , //BENE_GESSERIT
		ID_Wanna  , //BENE_GESSERIT
		ID_Irulan , //BENE_GESSERIT
		ID_Margot , //BENE_GESSERIT
		ID_Ramallo, //BENE_GESSERIT

		ID_Kudu   , //HARKONNEN
		ID_Nefud  , //HARKONNEN
		ID_Piter  , //HARKONNEN
		ID_Rabban , //HARKONNEN
		ID_Feyd   , //HARKONNEN

		ID_Bashar , //EMPEROR
		ID_Burseg , //EMPEROR
		ID_Caid   , //EMPEROR
		ID_Aramsha, //EMPEROR
		ID_Fenring, //EMPEROR

		ID_Repr   , //SPACING_GUILD
		ID_Sook   , //SPACING_GUILD
		ID_Esmar  , //SPACING_GUILD
		ID_Bewt   , //SPACING_GUILD
		ID_Staban , //SPACING_GUILD

		ID_Jamis  , //FREMEN
		ID_Mapes  , //FREMEN
		ID_Otheym , //FREMEN
		ID_Chani  , //FREMEN
		ID_Stilgar, //FREMEN

		ID_Zoal   , //TLEILAXU
		ID_Hidar  , //TLEILAXU
		ID_Zaaf   , //TLEILAXU
		ID_Wykk   , //TLEILAXU
		ID_Blin   , //TLEILAXU

		LEADERS_end
	};

	static const Leader INVALID;

	static const Leader& leader(Leader::Id id)
	{
		if (id < LEADERS_end)
			return leaders[id];
		else
			return INVALID;
	}

	Id id() const noexcept { return mId; }
	Faction faction() const noexcept { return mFaction; }

	static std::vector<Leader::Id> of(Faction faction);

private:
	static const Leader leaders[];

	constexpr Leader(Id id, Faction faction)
	: mId(id), mFaction(faction)
	{
	}

	const Id mId;
	const Faction mFaction;
};

using EnumLeaderId = Enum<Leader::Id, Leader::ID_INVALID>;

#endif

