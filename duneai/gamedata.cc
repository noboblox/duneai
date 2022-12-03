#include "gamedata.h"

#include <algorithm>
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
	if (drawPile.empty())
		reshuffle();

	drawn.push_back(peek());
	drawPile.pop_back();
	return drawn.back();
}

Leader::Id TraitorDeck::peek() const noexcept
{
	return drawPile.back();
}

void TraitorDeck::discard(Leader::Id card)
{
	for (auto it = drawn.begin(); it < drawn.end(); ++it)
	{
		if (*it == card)
		{
			discardPile.push_back(card);
			drawn.erase(it);
		}
	}
}

void TraitorDeck::reshuffle()
{
	drawPile.insert(drawPile.end(), discardPile.begin(), discardPile.end());
	discardPile.clear();

    std::shuffle(drawPile.begin(), drawPile.end(), *mpRandom);
}
