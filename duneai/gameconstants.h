#ifndef GAMECONSTANTS_H_
#define GAMECONSTANTS_H_

#include <string>

static constexpr int SEAT_A = 2;
static constexpr int SEAT_B = 5;
static constexpr int SEAT_C = 8;
static constexpr int SEAT_D = 11;
static constexpr int SEAT_E = 14;
static constexpr int SEAT_F = 17;

class Faction
{
public:
	static constexpr Faction emperor()                   { return Faction(CODE_EMPEROR); }
	static constexpr Faction spacingGuild()              { return Faction(CODE_SPACING_GUILD); }
	static constexpr Faction fremen()                    { return Faction(CODE_FREMEN); }
	static constexpr Faction atreides()                  { return Faction(CODE_ATREIDES); }
	static constexpr Faction harkonnen()                 { return Faction(CODE_HARKONNEN); }
	static constexpr Faction beneGesserit()              { return Faction(CODE_BENE_GESSERIT); }

	static constexpr Faction none()                      { return Faction(CODE_NONE); }
	static constexpr Faction any()                       { return Faction(CODE_ANY); }
	static constexpr Faction anyExcept(Faction excluded) { return Faction(any().mValue & (~excluded.mValue)); }

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

	Faction(const Faction& other)
	: mValue(other.mValue)
	{
	}

	bool operator==(Faction other) const noexcept { return mValue == other.mValue; }
	bool operator!=(Faction other) const noexcept { return !operator==(other); }

	Faction operator&(Faction other)
	{
		return Faction(mValue & other.mValue);
	}

	Faction operator|(Faction other)
	{
		return Faction(mValue | other.mValue);
	}

	void set(Faction added) noexcept
	{
		mValue |= added.mValue;
	}

	void clear(Faction removed) noexcept
	{
		mValue &= (~removed.mValue);
	}

	constexpr bool contains(Faction faction) const noexcept {
		return faction.mValue & mValue;
	}

private:
	static constexpr int CODE_ANY           = 0x003F;
	static constexpr int CODE_NONE          = 0;

	static constexpr int CODE_EMPEROR       = 0x0001;
	static constexpr int CODE_SPACING_GUILD = 0x0002;
	static constexpr int CODE_FREMEN        = 0x0004;
	static constexpr int CODE_ATREIDES      = 0x0008;
	static constexpr int CODE_HARKONNEN     = 0x0010;
	static constexpr int CODE_BENE_GESSERIT = 0x0020;

	static const char* codeLabel(Faction value)
	{
		switch (value.mValue)
		{
		case CODE_ANY          : return "any";
		case CODE_NONE         : return "none";
		case CODE_EMPEROR      : return "emperor";
		case CODE_SPACING_GUILD: return "spacingGuild";
		case CODE_FREMEN       : return "fremen";
		case CODE_ATREIDES     : return "atreides";
		case CODE_HARKONNEN    : return "harkonnen";
		case CODE_BENE_GESSERIT: return "beneGesserit";
		default:                 return "UNDEFINED";
		}
	}

	constexpr Faction(int value):  mValue(value) {}
	int mValue;
};


enum Leader
{
	LEADERS_begin,

	Yueh   = LEADERS_begin, //ATREIDES
	Duncan , //ATREIDES
	Gurney , //ATREIDES
	Thufir , //ATREIDES
	Jessica, //ATREIDES

	Alia   , //BENE_GESSERIT
	Wanna  , //BENE_GESSERIT
	Irulan , //BENE_GESSERIT
	Margot , //BENE_GESSERIT
	Ramallo, //BENE_GESSERIT

	Kudu   , //HARKONNEN
	Nefud  , //HARKONNEN
	Piter  , //HARKONNEN
	Rabban , //HARKONNEN
	Feyd   , //HARKONNEN

	Bashar , //EMPEROR
	Burseg , //EMPEROR
	Caid   , //EMPEROR
	Aramsha, //EMPEROR
	Fenring, //EMPEROR

	Repr   , //SPACING_GUILD
	Sook   , //SPACING_GUILD
	Esmar  , //SPACING_GUILD
	Bewt   , //SPACING_GUILD
	Staban , //SPACING_GUILD

	Jamis  , //FREMEN
	Mapes  , //FREMEN
	Otheym , //FREMEN
	Chani  , //FREMEN
	Stilgar, //FREMEN

	LEADERS_end
};


enum TreacheryCard
{
	LASGUN         ,
	CHRYS_KNIFE    ,
	MAULA_PISTOL   ,
	SLIP_TIP       ,
	STUNNER        ,
	CHAUMAS        ,
	CHAUMURKY      ,
	ELLACA_DRUG    ,
	GOM_JABBAR     ,
	SHIELD         ,
	SNOOPER        ,

	CHEAP_HERO     ,
	CHEAP_HEROINE  ,

	TLEILAXU_GHOLA ,
	FAMILY_ATOMICS ,
	HAJR           ,
	WEATHER_CONTROL,

	KARAMA         ,
	TRUTH_TRANCE   ,

	BALISET        ,
	JUBBA_CLOAK    ,
	KULON          ,
	LA_LA_LA       ,
	TRIP_TO_GAMONT ,
};

#endif

