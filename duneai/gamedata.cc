#include "gamedata.h"

#include "gameconstants.h"

template<>
std::unordered_map<TreacheryCard::Id, const char*> TreacheryCardLabels::labels =
{
	{ TreacheryCard::LASGUN         , "LASGUN"          },
	{ TreacheryCard::CRYS_KNIFE     , "CRYS_KNIFE"      },
	{ TreacheryCard::MAULA_PISTOL   , "MAULA_PISTOL"    },
	{ TreacheryCard::SLIP_TIP       , "SLIP_TIP"        },
	{ TreacheryCard::STUNNER        , "STUNNER"         },
	{ TreacheryCard::CHAUMAS        , "CHAUMAS"         },
	{ TreacheryCard::CHAUMURKY      , "CHAUMURKY"       },
	{ TreacheryCard::ELLACA_DRUG    , "ELLACA_DRUG"     },
	{ TreacheryCard::GOM_JABBAR     , "GOM_JABBAR"      },
	{ TreacheryCard::SHIELD_2       , "SHIELD_2"        },
	{ TreacheryCard::SHIELD_4       , "SHIELD_4"        },
	{ TreacheryCard::SHIELD_3       , "SHIELD_3"        },
	{ TreacheryCard::SHIELD_1       , "SHIELD_1"        },
	{ TreacheryCard::SNOOPER_1      , "SNOOPER_1"       },
	{ TreacheryCard::SNOOPER_2      , "SNOOPER_2"       },
	{ TreacheryCard::SNOOPER_3      , "SNOOPER_3"       },
	{ TreacheryCard::SNOOPER_4      , "SNOOPER_4"       },
	{ TreacheryCard::CHEAP_HERO_1   , "CHEAP_HERO_1"    },
	{ TreacheryCard::CHEAP_HERO_2   , "CHEAP_HERO_2"    },
	{ TreacheryCard::CHEAP_HEROINE  , "CHEAP_HEROINE"   },
	{ TreacheryCard::TLEILAXU_GHOLA , "TLEILAXU_GHOLA"  },
	{ TreacheryCard::FAMILY_ATOMICS , "FAMILY_ATOMICS"  },
	{ TreacheryCard::HAJR           , "HAJR"            },
	{ TreacheryCard::WEATHER_CONTROL, "WEATHER_CONTROL" },
	{ TreacheryCard::KARAMA_1       , "KARAMA_1"        },
	{ TreacheryCard::KARAMA_2       , "KARAMA_2"        },
	{ TreacheryCard::TRUTH_TRANCE_1 , "TRUTH_TRANCE_1"  },
	{ TreacheryCard::TRUTH_TRANCE_2 , "TRUTH_TRANCE_2"  },
	{ TreacheryCard::BALISET        , "BALISET"         },
	{ TreacheryCard::JUBBA_CLOAK    , "JUBBA_CLOAK"     },
	{ TreacheryCard::KULON          , "KULON"           },
	{ TreacheryCard::LA_LA_LA       , "LA_LA_LA"        },
	{ TreacheryCard::TRIP_TO_GAMONT , "TRIP_TO_GAMONT"  },
};

PlayerState PlayerState::create(int aSeat, Faction aFaction)
{
	if (aFaction == Faction::emperor())
		return PlayerState(aSeat, Faction::emperor(),       10, 15, 5, 4, Leader::of(Faction::emperor()));
	if (aFaction == Faction::spacingGuild())
		return PlayerState(aSeat, Faction::spacingGuild(),  5,  20, 0, 4, Leader::of(Faction::spacingGuild()));
	if (aFaction == Faction::atreides())
		return PlayerState(aSeat, Faction::atreides(),      10, 20, 0, 4, Leader::of(Faction::atreides()));
	if (aFaction == Faction::harkonnen())
		return PlayerState(aSeat, Faction::harkonnen(),     10, 20, 0, 8, Leader::of(Faction::harkonnen()));
	if (aFaction == Faction::fremen())
		return PlayerState(aSeat, Faction::fremen(),        3,  17, 3, 4, Leader::of(Faction::fremen()));
	if (aFaction == Faction::beneGesserit())
		return PlayerState(aSeat, Faction::beneGesserit(),  5,  20, 0, 4, Leader::of(Faction::beneGesserit()));
	if (aFaction == Faction::tleilaxu())
		return PlayerState(aSeat, Faction::tleilaxu(),      5,  20, 0, 4, Leader::of(Faction::tleilaxu()));
	else
		return PlayerState();
}

TraitorDeck::TraitorDeck(Faction factionsInGame, std::mt19937& random)
: mpRandom(&random)
{
	for (int i = static_cast<int> (Leader::LEADERS_begin); i < Leader::LEADERS_end; ++i)
	{
		drawPile.push_back(static_cast<Leader::Id> (i));
	}

    std::shuffle(drawPile.begin(), drawPile.end(), random);
}


Leader::Id TraitorDeck::draw()
{
	Leader::Id result = peek();
	drawPile.pop_back();

	if (drawPile.empty())
		reshuffle();

	return result;
}

Leader::Id TraitorDeck::peek() const
{
	return drawPile.back();
}

void TraitorDeck::discard(Leader::Id card)
{
	discardPile.push_back(card);
}

void TraitorDeck::reshuffle()
{
	drawPile.insert(drawPile.end(), discardPile.begin(), discardPile.end());
	discardPile.clear();
    std::shuffle(drawPile.begin(), drawPile.end(), *mpRandom);
}


SpiceDeck::SpiceDeck(std::mt19937& random)
: mpRandom(&random)
{
	drawPile.emplace_back(SpiceCard::CIELAGO_SOUTH      , CielagoSouth_2,       12);
	drawPile.emplace_back(SpiceCard::SOUTH_MESA         , SouthMesa_5,          10);
	drawPile.emplace_back(SpiceCard::RED_CHASM          , RedChasm,             8 );
	drawPile.emplace_back(SpiceCard::BROKEN_LAND        , BrokenLand_12,        8 );
	drawPile.emplace_back(SpiceCard::OLD_GAP            , OldGap_10,            6 );
	drawPile.emplace_back(SpiceCard::SIHAYA_RIDGE       , SihayaRidge,          6 );
	drawPile.emplace_back(SpiceCard::ROCK_OUTCROPPINGS  , RockOutcroppings_14,  6 );
	drawPile.emplace_back(SpiceCard::FUNERAL_PLAIN      , FuneralPlain,         6 );
	drawPile.emplace_back(SpiceCard::THE_GREAT_FLAT     , TheGreatFlat,         10);
	drawPile.emplace_back(SpiceCard::CIELAGO_NORTH      , CielagoNorth_3,       8 );
	drawPile.emplace_back(SpiceCard::HABBANYA_RIDGE_FLAT, HabbanyaRidgeFlat_18, 10);
	drawPile.emplace_back(SpiceCard::HABBANYA_ERG       , HabbanyaErg_16,       8 );
	drawPile.emplace_back(SpiceCard::THE_MINOR_ERG      , TheMinorErg_8,        8 );
	drawPile.emplace_back(SpiceCard::HAGGA_BASIN        , HaggaBasin_13,        6 );
	drawPile.emplace_back(SpiceCard::WIND_PASS_NORTH    , WindPassNorth_17,     6 );
	drawPile.emplace_back(SpiceCard::SHAI_HULUD         , AreaId::INVALID,      0 );
	drawPile.emplace_back(SpiceCard::SHAI_HULUD         , AreaId::INVALID,      0 );
	drawPile.emplace_back(SpiceCard::SHAI_HULUD         , AreaId::INVALID,      0 );
	drawPile.emplace_back(SpiceCard::SHAI_HULUD         , AreaId::INVALID,      0 );
	drawPile.emplace_back(SpiceCard::SHAI_HULUD         , AreaId::INVALID,      0 );
	drawPile.emplace_back(SpiceCard::SHAI_HULUD         , AreaId::INVALID,      0 );

    std::shuffle(drawPile.begin(), drawPile.end(), random);
}

SpiceCard SpiceDeck::draw()
{
	SpiceCard result = peekDraw();
	drawPile.pop_back();

	if (drawPile.empty())
		reshuffle();

	return result;
}

static SpiceCard safePeek(const std::vector<SpiceCard>& pile)
{
	if (pile.empty())
		return SpiceCard();
	return pile.back();
}

SpiceCard SpiceDeck::peekDraw() const
{
	return safePeek(drawPile);
}

SpiceCard SpiceDeck::peekDiscardA() const
{
	return safePeek(discardPileA);
}

SpiceCard SpiceDeck::peekDiscardB() const
{
	return safePeek(discardPileB);
}

void SpiceDeck::placeDrawTop(SpiceCard card)
{
	drawPile.push_back(card);
}

void SpiceDeck::placeDrawBottom(SpiceCard card)
{
	drawPile.insert(drawPile.begin(), card);
}

void SpiceDeck::discardA(SpiceCard card)
{
	discardPileA.push_back(card);
}

void SpiceDeck::discardB(SpiceCard card)
{
	discardPileB.push_back(card);
}

static void recycleDiscardPile(std::vector<SpiceCard>& discardPile, std::vector<SpiceCard>& drawPile)
{
	SpiceCard savedTopCard = safePeek(discardPile);

	if (!discardPile.empty())
		discardPile.pop_back();

	drawPile.insert(drawPile.end(), discardPile.begin(), discardPile.end());
	discardPile.clear();

	if (savedTopCard.id() != SpiceCard::INVALID)
		discardPile.push_back(savedTopCard);
}

void SpiceDeck::reshuffle()
{
	recycleDiscardPile(discardPileA, drawPile);
	recycleDiscardPile(discardPileB, drawPile);
	std::shuffle(drawPile.begin(), drawPile.end(), *mpRandom);
}

TreacheryDeck::TreacheryDeck(std::mt19937& random)
: mpRandom(&random),
  drawPile{
	TreacheryCard{TreacheryCard::LASGUN         , TreacheryCard::P_LASGUN | TreacheryCard::P_PROJECTILE_WEAPON | TreacheryCard::P_POISON_WEAPON },
    TreacheryCard{TreacheryCard::CRYS_KNIFE     , TreacheryCard::P_PROJECTILE_WEAPON},
    TreacheryCard{TreacheryCard::MAULA_PISTOL   , TreacheryCard::P_PROJECTILE_WEAPON},
    TreacheryCard{TreacheryCard::SLIP_TIP       , TreacheryCard::P_PROJECTILE_WEAPON},
    TreacheryCard{TreacheryCard::STUNNER        , TreacheryCard::P_PROJECTILE_WEAPON},
    TreacheryCard{TreacheryCard::CHAUMAS        , TreacheryCard::P_POISON_WEAPON},
    TreacheryCard{TreacheryCard::CHAUMURKY      , TreacheryCard::P_POISON_WEAPON},
    TreacheryCard{TreacheryCard::ELLACA_DRUG    , TreacheryCard::P_POISON_WEAPON},
    TreacheryCard{TreacheryCard::GOM_JABBAR     , TreacheryCard::P_POISON_WEAPON},
    TreacheryCard{TreacheryCard::SHIELD_1       , TreacheryCard::P_PROJECTILE_DEFENSE},
    TreacheryCard{TreacheryCard::SHIELD_2       , TreacheryCard::P_PROJECTILE_DEFENSE},
    TreacheryCard{TreacheryCard::SHIELD_3       , TreacheryCard::P_PROJECTILE_DEFENSE},
    TreacheryCard{TreacheryCard::SHIELD_4       , TreacheryCard::P_PROJECTILE_DEFENSE},
    TreacheryCard{TreacheryCard::SNOOPER_1      , TreacheryCard::P_POISON_DEFENSE},
    TreacheryCard{TreacheryCard::SNOOPER_2      , TreacheryCard::P_POISON_DEFENSE},
    TreacheryCard{TreacheryCard::SNOOPER_3      , TreacheryCard::P_POISON_DEFENSE},
    TreacheryCard{TreacheryCard::SNOOPER_4      , TreacheryCard::P_POISON_DEFENSE},
    TreacheryCard{TreacheryCard::CHEAP_HERO_1   , TreacheryCard::P_HERO},
    TreacheryCard{TreacheryCard::CHEAP_HERO_2   , TreacheryCard::P_HERO},
    TreacheryCard{TreacheryCard::CHEAP_HEROINE  , TreacheryCard::P_HERO},
    TreacheryCard{TreacheryCard::TLEILAXU_GHOLA , TreacheryCard::P_SPECIAL_EFFECT},
    TreacheryCard{TreacheryCard::FAMILY_ATOMICS , TreacheryCard::P_SPECIAL_EFFECT},
    TreacheryCard{TreacheryCard::HAJR           , TreacheryCard::P_SPECIAL_EFFECT},
    TreacheryCard{TreacheryCard::WEATHER_CONTROL, TreacheryCard::P_SPECIAL_EFFECT},
    TreacheryCard{TreacheryCard::KARAMA_1       , TreacheryCard::P_KARAMA},
    TreacheryCard{TreacheryCard::KARAMA_2       , TreacheryCard::P_KARAMA},
    TreacheryCard{TreacheryCard::TRUTH_TRANCE_1 , TreacheryCard::P_TRUTHTRANCE},
    TreacheryCard{TreacheryCard::TRUTH_TRANCE_2 , TreacheryCard::P_TRUTHTRANCE},
    TreacheryCard{TreacheryCard::BALISET        , TreacheryCard::P_WORTHLESS},
    TreacheryCard{TreacheryCard::JUBBA_CLOAK    , TreacheryCard::P_WORTHLESS},
    TreacheryCard{TreacheryCard::KULON          , TreacheryCard::P_WORTHLESS},
    TreacheryCard{TreacheryCard::LA_LA_LA       , TreacheryCard::P_WORTHLESS},
    TreacheryCard{TreacheryCard::TRIP_TO_GAMONT , TreacheryCard::P_WORTHLESS}}
{
    std::shuffle(drawPile.begin(), drawPile.end(), random);
}


void TreacheryDeck::placeOnTop(const TreacheryCard& card)
{
	drawPile.push_back(card);
}

TreacheryCard TreacheryDeck::draw()
{
	TreacheryCard result = peek();
	drawPile.pop_back();

	if (drawPile.empty())
		reshuffle();

	return result;
}

TreacheryCard TreacheryDeck::peek() const
{
	return drawPile.back();
}

void TreacheryDeck::discard(TreacheryCard card)
{
	discardPile.push_back(card);
}

void TreacheryDeck::reshuffle()
{
	drawPile.insert(drawPile.end(), discardPile.begin(), discardPile.end());
	discardPile.clear();
    std::shuffle(drawPile.begin(), drawPile.end(), *mpRandom);
}

PlayerState*
getPlayerState(GameState& game, Faction faction)
{
	auto it = std::find_if(game.players.begin(), game.players.end(),
				[faction] (const PlayerState& s) -> bool { return s.faction == faction; });
	if (it != game.players.end())
		return &(*it);
	else
		return nullptr;
}
