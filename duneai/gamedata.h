#ifndef GAMEDATA_H_
#define GAMEDATA_H_

#include <algorithm>
#include <random>
#include <vector>

#include "arrakis.h"
#include "gameconstants.h"

class TraitorDeck
{
public:
	explicit TraitorDeck() : mpRandom(nullptr) {}
	explicit TraitorDeck(Faction factionsInGame, std::mt19937& random);

	Leader::Id draw();
	Leader::Id peek() const;
	void discard(Leader::Id card);
	void reshuffle();

private:
	std::vector<Leader::Id> drawPile;
	std::vector<Leader::Id> discardPile;
	std::mt19937* mpRandom;
};



class SpiceCard
{
public:
	enum Id
	{
		INVALID,

	    CIELAGO_SOUTH,
	    SOUTH_MESA,
	    RED_CHASM,
	    BROKEN_LAND,
	    OLD_GAP,
	    SIHAYA_RIDGE,
	    ROCK_OUTCROPPINGS,
	    FUNERAL_PLAIN,
	    THE_GREAT_FLAT,
	    CIELAGO_NORTH,
	    HABBANYA_RIDGE_FLAT,
	    HABBANYA_ERG,
	    THE_MINOR_ERG,
	    HAGGA_BASIN,
	    WIND_PASS_NORTH,
	    SHAI_HULUD,
	};

	SpiceCard()
	: mId(SpiceCard::INVALID), mArea(AreaId::INVALID), mBaseValue(0)
	{
	}

	SpiceCard(SpiceCard::Id aId, AreaId aArea, int aBase)
	: mId (aId), mArea(aArea), mBaseValue(aBase)
	{
	}

	static void swap(SpiceCard& l, SpiceCard& r) noexcept
	{
		std::swap(l.mId, r.mId);
		std::swap(l.mArea, r.mArea);
		std::swap(l.mBaseValue, r.mBaseValue);
	}

	Id     id()   const noexcept { return mId; }
	AreaId area() const noexcept { return mArea; }
	int    base() const noexcept { return mBaseValue; }

private:
	Id mId;
	AreaId mArea;
	int mBaseValue;
};


class SpiceDeck
{
public:
	explicit SpiceDeck() : mpRandom(nullptr) {}
	explicit SpiceDeck(std::mt19937& random);

	SpiceCard draw();

	SpiceCard peekDraw() const;
	SpiceCard peekDiscardA() const;
	SpiceCard peekDiscardB() const;

	void placeDrawTop(SpiceCard card);
	void placeDrawBottom(SpiceCard card);

	void discardA(SpiceCard card);
	void discardB(SpiceCard card);

	void reshuffle();

private:
	std::vector<SpiceCard> drawPile;
	std::vector<SpiceCard> discardPileA;
	std::vector<SpiceCard> discardPileB;
	std::mt19937* mpRandom;
};




struct PlayerState
{
	static PlayerState create(int aSeat, Faction aFaction);

	PlayerState()
	: seat(0), faction(Faction::none()), spice(0), reserve(0), specialForcesReserve(0)
	{
	}

	PlayerState(int aSeat, Faction aFaction, int aSpice, int aReserve, int aSpecialForces, std::vector<Leader::Id>&& leaders)
	: seat(aSeat), faction(aFaction), spice(aSpice), reserve(aReserve), specialForcesReserve(aSpecialForces),
	  alive(leaders)
	{
	}

	int seat;
	Faction faction;

	int spice;
	int reserve;
	int specialForcesReserve;

	std::vector<Leader::Id> alive;
	std::vector<Leader::Id> selectedTraitors;
	std::vector<TreacheryCard> hand;

};

struct PublicGameState
{
	int round = 1;
	int maxRound = 10;
	GamePhase phase = PHASE_invalid;

	Faction expectingInputFrom = Faction::none();
	Arrakis board;
};


struct GameState : public PublicGameState
{
	static constexpr int VERSION = 1;

	unsigned int seed = 0;
	std::mt19937 random;

	int predictedTurn = 0;
	Faction predictedFaction = Faction::none();

	TraitorDeck traitors;
	SpiceDeck spiceDeck;
	std::vector<PlayerState> players;

	std::pair<Faction, int> initialStormDial[2] = { {Faction::none(), 0}, {Faction::none(), 0}};
};

#endif /* GAMEDATA_H_ */
