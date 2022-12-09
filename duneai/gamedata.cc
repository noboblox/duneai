#include "gamedata.h"

#include "gameconstants.h"

PlayerState PlayerState::create(int aSeat, Faction aFaction)
{
	if (aFaction == Faction::emperor())
		return PlayerState(aSeat, Faction::emperor(),       10, 15, 5, Leader::of(Faction::emperor()));
	if (aFaction == Faction::spacingGuild())
		return PlayerState(aSeat, Faction::spacingGuild(),  5,  20, 0, Leader::of(Faction::spacingGuild()));
	if (aFaction == Faction::atreides())
		return PlayerState(aSeat, Faction::atreides(),      10, 20, 0, Leader::of(Faction::atreides()));
	if (aFaction == Faction::harkonnen())
		return PlayerState(aSeat, Faction::harkonnen(),     10, 20, 0, Leader::of(Faction::harkonnen()));
	if (aFaction == Faction::fremen())
		return PlayerState(aSeat, Faction::fremen(),        3,  15, 5, Leader::of(Faction::fremen()));
	if (aFaction == Faction::beneGesserit())
		return PlayerState(aSeat, Faction::beneGesserit(),  5,  20, 0, Leader::of(Faction::beneGesserit()));
	if (aFaction == Faction::tleilaxu())
		return PlayerState(aSeat, Faction::tleilaxu(),      5,  20, 0, Leader::of(Faction::tleilaxu()));
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
