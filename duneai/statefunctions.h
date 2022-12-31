#include "gamedata.h"

class StateFunctions
{
public:
    static bool ownsCard(const PlayerState& player, TreacheryCard::Id card);
    static bool discardCard(PlayerState& player, TreacheryCard::Id card, TreacheryDeck& deck);

    static int  aliveHeros(const PlayerState& player);
    static bool ownsAliveHero(const PlayerState& player, Leader::Id card);
    static bool ownsCheapHero(const PlayerState& player);
};
