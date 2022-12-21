#ifndef GAMECONSTANTS_H_
#define GAMECONSTANTS_H_

#include <string>
#include <vector>
#include "enumlabels.h"
/// enum containing all stages of the game
enum GamePhase
{
	PHASE_invalid    = 0,

	PHASE_INIT_PREDICTION = 1,
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
	PHASE_BATTLE,
};
using GamePhaseLabels = EnumLabels<GamePhase>;

enum AreaId
{
	INVALID                = 0,

	PolarSink              = 10 ,
	FalseWallEast_5        = 20 ,
	FalseWallEast_6        = 21 ,
	FalseWallEast_7        = 22 ,
	FalseWallEast_8        = 23 ,
	FalseWallEast_9        = 24 ,
	TheMinorErg_5          = 30 ,
	TheMinorErg_6          = 31 ,
	TheMinorErg_7          = 32 ,
	TheMinorErg_8          = 33 ,
	PastyMesa_5            = 40 ,
	PastyMesa_6            = 41 ,
	PastyMesa_7            = 42 ,
	PastyMesa_8            = 43 ,
	RedChasm               = 50 ,
	ShieldWall_8           = 60 ,
	ShieldWall_9           = 61 ,
	SihayaRidge            = 70 ,
	HoleInTheRock          = 80 ,
	Basin                  = 90 ,
	RimWallWest            = 100,
	ImperialBasin_9        = 110,
	ImperialBasin_10       = 111,
	ImperialBasin_11       = 112,
	GaraKulon              = 120,
	Arrakeen               = 130,
	OldGap_9               = 140,
	OldGap_10              = 141,
	OldGap_11              = 142,
	Carthag                = 150,
	Tsimpo_11              = 160,
	Tsimpo_12              = 161,
	Tsimpo_13              = 162,
	BrokenLand_11          = 170,
	BrokenLand_12          = 171,
	Arsunt_11              = 180,
	Arsunt_12              = 181,
	PlasticBasin_12        = 190,
	PlasticBasin_13        = 191,
	PlasticBasin_14        = 192,
	HaggaBasin_12          = 200,
	HaggaBasin_13          = 201,
	RockOutcroppings_13    = 210,
	RockOutcroppings_14    = 211,
	WindPass_14            = 220,
	WindPass_15            = 221,
	WindPass_16            = 222,
	WindPass_17            = 223,
	SietchTabr             = 230,
	BightOfTheCliff_14     = 240,
	BightOfTheCliff_15     = 241,
	FuneralPlain           = 250,
	TheGreatFlat           = 260,
	TheGreaterFlat         = 270,
	HabbanyaErg_16         = 280,
	HabbanyaErg_17         = 281,
	FalseWallWest_16       = 290,
	FalseWallWest_17       = 291,
	FalseWallWest_18       = 292,
	HabbanyaRidgeFlat_17   = 300,
	HabbanyaRidgeFlat_18   = 301,
	CielagoWest_18         = 310,
	CielagoWest_1          = 311,
	WindPassNorth_17       = 320,
	WindPassNorth_18       = 321,
	CielagoNorth_1         = 330,
	CielagoNorth_2         = 331,
	CielagoNorth_3         = 332,
	HargPass_4             = 340,
	HargPass_5             = 341,
	HabbanyaSietch         = 350,
	FalseWallSouth_4       = 360,
	FalseWallSouth_5       = 361,
	Meridian_1             = 370,
	Meridian_2             = 371,
	CielagoDepression_1    = 380,
	CielagoDepression_2    = 381,
	CielagoDepression_3    = 382,
	CielagoSouth_2         = 390,
	CielagoSouth_3         = 391,
	CielagoEast_3          = 400,
	CielagoEast_4          = 401,
	SouthMesa_4            = 410,
	SouthMesa_5            = 411,
	SouthMesa_6            = 412,
	TueksSietch            = 420,
};

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

private:
	static constexpr int CODE_ANY           = 0x007F;
	static constexpr int CODE_NONE          = 0;

	static constexpr int CODE_EMPEROR       = 0x0001;
	static constexpr int CODE_SPACING_GUILD = 0x0002;
	static constexpr int CODE_FREMEN        = 0x0004;
	static constexpr int CODE_ATREIDES      = 0x0008;
	static constexpr int CODE_HARKONNEN     = 0x0010;
	static constexpr int CODE_BENE_GESSERIT = 0x0020;
	static constexpr int CODE_TLEILAXU      = 0x0040;

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
		LEADERS_begin = 0,

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

	static const char* name(Leader::Id id)
    {
		return leader(id).name();
	}

	static const Leader& leader(Leader::Id id)
	{
		if (id < LEADERS_end)
			return leaders[id];
		else
			return INVALID;
	}

	Id id() const noexcept { return mId; }
	Faction faction() const noexcept { return mFaction; }
	const char* name() const noexcept { return mName; }

	static std::vector<Leader::Id> of(Faction faction);

private:
	static const Leader leaders[];

	constexpr Leader(Id id, const char* name, Faction faction)
	: mId(id), mFaction(faction), mName(name)
	{
	}

	const Id mId;
	const Faction mFaction;
	const char* mName;
};

#endif

