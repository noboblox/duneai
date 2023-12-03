#pragma once

#include <future>

#include "areaid.h"
#include "gameconstants.h"
#include "resultcode.h"

class BattlePlan;
class Placement;

class IPlayerActions
{
public:
	virtual ~IPlayerActions() {}

	virtual std::future<ResultCode> predictWinner(Faction winner, int round) = 0;
	virtual std::future<ResultCode> selectTraitor(Leader::Id selection) = 0;
	virtual std::future<ResultCode> setupFremen(std::vector<Placement>&& placements) = 0;
	virtual std::future<ResultCode> decideTraitorRedraw(bool decision) = 0;
	virtual std::future<ResultCode> selectBeneGesseritStartArea(AreaId where) = 0;
	virtual std::future<ResultCode> dialStorm(int dial) = 0;
	virtual std::future<ResultCode> decideChoamCharity(bool decision) = 0;
	virtual std::future<ResultCode> bid(BiddingAction action, int amount) = 0;
	virtual std::future<ResultCode> decideGuildTurn(bool decision) = 0;
	virtual std::future<ResultCode> shipFromReserve(Placement to) = 0;
	virtual std::future<ResultCode> shipCrossPlanet(AreaId from, Placement to) = 0;
	virtual std::future<ResultCode> shipToReserve(Placement from) = 0;
	virtual std::future<ResultCode> reactToIntrusion(bool disengage) = 0;
	virtual std::future<ResultCode> accompanyShipment(bool decision, bool toPolarSink) = 0;
	virtual std::future<ResultCode> move(AreaId aFrom, Placement aTo, bool aUseHajr) = 0;
	virtual std::future<ResultCode> moveAsAdvisor(AreaId aFrom, Placement aTo, bool aUseHajr) = 0;
	virtual std::future<ResultCode> selectBattle(int id) = 0;
	virtual std::future<ResultCode> commitBattlePlan(BattlePlan&& battlePlan) = 0;
};
