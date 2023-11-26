#pragma once

#include "IPlayerActions.h"
#include "messagethread.h"
#include "deletedefaults.h"
#include <memory>

class Action;

class StandardPlayerClient : public IPlayerActions
{
public:
	MACRO_DELETE_ALL_DEFAULTS(StandardPlayerClient)
	explicit StandardPlayerClient(Faction own);

	std::future<ResultCode> predictWinner(Faction winner, int round) override;
	std::future<ResultCode> selectTraitor(Leader::Id selection) override;
	std::future<ResultCode> setupFremen(std::vector<Placement>&& placements) override;
	std::future<ResultCode> decideTraitorRedraw(bool decision) override;
	std::future<ResultCode> selectBeneGesseritStartArea(AreaId where) override;
	std::future<ResultCode> dialStorm(int dial) override;
	std::future<ResultCode> decideChoamCharity(bool decision) override;
	std::future<ResultCode> bid(BiddingAction action, int amount) override;
	std::future<ResultCode> decideGuildTurn(bool decision) override;
	std::future<ResultCode> shipFromReserve(Placement to) override;
	std::future<ResultCode> shipCrossPlanet(AreaId from, Placement to) override;
	std::future<ResultCode> shipToReserve(Placement from) override;
	std::future<ResultCode> reactToIntrusion(bool disengage) override;
	std::future<ResultCode> accompanyShipment(bool decision, bool toPolarSink) override;
	std::future<ResultCode> move(AreaId aFrom, Placement aTo, bool aUseHajr) override;
	std::future<ResultCode> moveAsAdvisor(AreaId aFrom, Placement aTo, bool aUseHajr) override;
	std::future<ResultCode> selectBattle(int id) override;
	std::future<ResultCode> commitBattlePlan(BattlePlan&& battlePlan) override;

private:
	std::future<ResultCode> sendAction(std::unique_ptr<Action>&& action);

	Faction mFaction;

};