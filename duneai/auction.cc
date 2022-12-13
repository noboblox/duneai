/*
 * Auction.cpp
 *
 *  Created on: 10.12.2022
 *      Author: Rene
 */

#include "auction.h"
#include "gamedata.h"

Auction::Auction()
: round(0), lastRound(0), indexBidder(0), indexHighestBidder(0), indexWinner(NO_WINNER)
{
	data.push_back(AuctionData{0, 0, 0, Faction::none()});
}

Auction::Auction(const GameState& state)
: round(1), lastRound(0), indexBidder(0), indexHighestBidder(0), indexWinner(NO_WINNER)
{
	auto turnOrder = state.board.stormOrder();

	for (const auto& seat : turnOrder)
	{
		auto it = std::find_if(state.players.cbegin(), state.players.cend(),
				[seat](const PlayerState& p) -> bool { return p.seat == seat; });
		data.push_back(AuctionData{it->maxHand, (int) it->hand.size(), 0, it->faction});
	}

	lastRound = eligible();
}

int Auction::eligible() const noexcept
{
	int eligible = 0;
	std::for_each(data.cbegin(), data.cend(),
			[&eligible](const AuctionData& d){ if (d.maxCards > d.cards) ++eligible; });
	return eligible;
}

Faction Auction::currentBidder() const noexcept
{
	return data[indexBidder].faction;
}

int Auction::currentBid() const noexcept
{
	return data[indexHighestBidder].bid;
}

bool Auction::roundFinished() const noexcept
{
	return indexWinner != NO_WINNER;
}

void Auction::leftShift(std::vector<AuctionData>& data)
{
	std::rotate(data.begin(), ++data.begin(), data.end());
}

void Auction::nextIndex() noexcept
{
	do
	{
		++indexBidder;
		indexBidder %= data.size();
	} while (hasFullHand(data[indexBidder]));
}

bool Auction::hasFullHand(const AuctionData& data) const noexcept
{
	return data.maxCards <= data.cards;
}


bool Auction::nextRound() noexcept
{
	if (round >= lastRound)
		return false;
	if (!eligible())
		return false;
	if (roundFinished() && winner() == Faction::none())
		return false;

	++round;
	indexBidder = 0;
	indexHighestBidder = 0;
	indexWinner = NO_WINNER;

	do
	{
		leftShift(data);
	} while (hasFullHand(data[0]));

	std::for_each(data.begin(), data.end(),
			[](AuctionData& d){ d.bid = 0; });

	return true;
}

void Auction::bid(int value) noexcept
{
	if (indexWinner != NO_WINNER)
		return;
	if (value <= data[indexHighestBidder].bid)
	{
		pass();
		return;
	}

	data[indexBidder].bid = value;
	indexHighestBidder = indexBidder;
	nextIndex();
}

void Auction::karamaWin() noexcept
{
	if (indexWinner != NO_WINNER)
		return;

	indexWinner = indexBidder;
	data[indexBidder].bid = AuctionData::KARAMA;
}

void Auction::pass() noexcept
{
	if (indexWinner != NO_WINNER)
		return;

	nextIndex();

	if (indexHighestBidder == indexBidder)
	{
		indexWinner = indexBidder;
	}
}

Faction Auction::winner() const noexcept
{
	if (indexWinner == NO_WINNER)
		return Faction::none();
	else if (data[indexBidder].bid == 0)
		return Faction::none();
	else
		return data[indexWinner].faction;
}

int Auction::winningCost() const noexcept
{
	if (indexWinner == NO_WINNER)
		return 0;
	else if (wasKaramaWin())
		return 0;
	else
		return data[indexWinner].bid;
}

bool Auction::wasKaramaWin() const noexcept
{
	if (indexWinner == NO_WINNER)
		return false;
	else
		return data[indexWinner].bid == AuctionData::KARAMA;
}


