#ifndef GAMEDATA_H_
#define GAMEDATA_H_

#include <random>
#include <vector>

#include "arrakis.h"
#include "gameconstants.h"

struct Placement
{
	Arrakis::AreaId where = Arrakis::PolarSink;
	int normal = 0;
	int special = 0;
};

struct ForcesFrom
{
	Faction from = Faction::none();
	Placement forces;
};

class TraitorDeck
{
public:
	explicit TraitorDeck() : mpRandom(nullptr) {}
	explicit TraitorDeck(Faction factionsInGame, std::mt19937& random);

	Leader::Id draw();
	Leader::Id peek() const noexcept;
	void discard(Leader::Id card);
	void reshuffle();

private:
	std::mt19937* mpRandom;
	std::vector<Leader::Id> drawPile;
	std::vector<Leader::Id> discardPile;
	std::vector<Leader::Id> drawn;
};

struct PlayerState
{
	static PlayerState create(int aSeat, Faction aFaction);

	PlayerState()
	: seat(0), faction(Faction::none()), spice(0), reserve(0), specialForcesReserve(0)
	{
	}

	PlayerState(int aSeat, Faction aFaction, int aSpice, int aReserve, int aSpecialForces)
	: seat(aSeat), faction(aFaction), spice(aSpice), reserve(aReserve), specialForcesReserve(aSpecialForces)
	{
	}

	int seat;
	Faction faction;

	int spice;
	int reserve;
	int specialForcesReserve;

	std::vector<Leader::Id> alive;
	std::vector<Leader::Id> selectedTraitors;
	std::vector<Leader::Id> discardedTraitors;
	std::vector<TreacheryCard> hand;

};

struct GameState
{
	int round = 1;
	unsigned int seed = 0;
	std::mt19937 random;

	GamePhase phase = PHASE_invalid;
	Faction expectingInputFrom = Faction::none();

	int predictedTurn = 0;
	Faction predictedFaction = Faction::none();

	TraitorDeck traitors;
	std::vector<PlayerState> players;
	std::vector<ForcesFrom> forces;
};

#endif /* GAMEDATA_H_ */
