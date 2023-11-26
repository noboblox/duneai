#ifndef AUCTION_H_
#define AUCTION_H_

#include "gameconstants.h"

class GameState;

class Auction
{
public:
	explicit Auction();
	explicit Auction(const GameState& state);

	int eligible() const noexcept;
	Faction currentBidder() const noexcept;
	int currentBid() const noexcept;

	bool roundFinished() const noexcept;
	bool nextRound() noexcept;

	void bid(int value, bool withKarama = false) noexcept;
	void karamaWin() noexcept;
	void pass() noexcept;

	int currentRound() const noexcept { return round; }
	Faction winner() const noexcept;
	int winningCost() const noexcept;
	bool wasKaramaWin() const noexcept;

	void cardReceived() noexcept;
	void cardDiscarded() noexcept;

private:
	static constexpr int NO_WINNER = -1;

	struct AuctionData
	{
		static constexpr int KARAMA_INSTANT_BUY = -2;

		int maxCards;
		int cards;
		int bid;
		bool withKarama;
		Faction faction;
	};

	int round;
	int lastRound;
	int indexBidder;
	int indexHighestBidder;
	int indexWinner;

	std::vector<AuctionData> data;

	void leftShift(std::vector<AuctionData>& data);
	void nextIndex() noexcept;
	bool hasFullHand(const AuctionData& data) const noexcept;
};

#endif
