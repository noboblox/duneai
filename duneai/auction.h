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

	void bid(int value) noexcept;
	void karamaWin() noexcept;
	void pass() noexcept;

	Faction winner() const noexcept;
	int winningCost() const noexcept;
	bool wasKaramaWin() const noexcept;

private:
	static constexpr int NO_WINNER = -1;

	struct AuctionData
	{
		static constexpr int KARAMA = -2;

		int maxCards;
		int cards;
		int bid;
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
