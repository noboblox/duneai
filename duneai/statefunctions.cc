#include "statefunctions.h"

std::vector<TreacheryCard>::const_iterator findCard(const PlayerState& player, TreacheryCard::Id card)
{
	return std::find_if(player.hand.begin(), player.hand.end(),
		[card](const TreacheryCard& c) { return c.id() == card; });
}

std::vector<Leader::Id>::const_iterator findHero(const PlayerState& player, Leader::Id hero)
{
	return std::find_if(player.alive.begin(), player.alive.end(),
		[hero](const Leader::Id& h) { return h == hero; });
}

bool StateFunctions::ownsCard(const PlayerState& player, TreacheryCard::Id card)
{
	return findCard(player, card) != player.hand.end();
}

bool StateFunctions::discardCard(PlayerState& player, TreacheryCard::Id card, TreacheryDeck& deck)
{
	auto it = findCard(player, card);

	if (it == player.hand.end())
		return false;

	deck.discard(*it);
	player.hand.erase(it);
	return true;
}

int StateFunctions::aliveHeros(const PlayerState& player)
{
	return (int) player.alive.size();
}

bool StateFunctions::ownsAliveHero(const PlayerState& player, Leader::Id hero)
{
	return findHero(player, hero) != player.alive.end();
}

bool StateFunctions::ownsCheapHero(const PlayerState& player)
{
	return std::find_if(player.hand.cbegin(), player.hand.cend(),
		[](const TreacheryCard& c) { return c.isCheapHero(); }) != player.hand.cend();;
}
