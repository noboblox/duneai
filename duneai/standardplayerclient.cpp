#include "standardplayerclient.h"
#include "gamedata.h"

#include "actions.h"
#include "broker.h"
#include "game.h"

StandardPlayerClient::StandardPlayerClient(Faction own, Broker& broker, const Game& game)
: StandardPlayerClient(own, broker, game, false)
{
}

StandardPlayerClient::StandardPlayerClient(Faction own, Broker& broker, const Game& game, bool asGameMaster)
: MessageThread(broker),
  mFaction(own),
  mGameId(game.actorId())
{
	if (!own.exactlyOne())
		throw std::invalid_argument("StandardPlayerClient must represent exactly one faction");

	connect(asGameMaster);
}

StandardPlayerClient::~StandardPlayerClient() noexcept
{
	disconnect();
}

std::future<ResultCode> StandardPlayerClient::predictWinner(Faction winner, int round)
{
	return sendAction(std::make_unique<ActionPrediction>(mFaction, winner, round));
}

std::future<ResultCode> StandardPlayerClient::selectTraitor(Leader::Id selection)
{
	return sendAction(std::make_unique<ActionTraitorSelection>(mFaction, selection));
}

std::future<ResultCode> StandardPlayerClient::setupFremen(std::vector<Placement>&& placements)
{
	//throw std::invalid_argument("not implemented");
	return sendAction(std::make_unique<ActionFremenPlacement>(mFaction, std::move(placements)));
}

std::future<ResultCode> StandardPlayerClient::decideTraitorRedraw(bool decision)
{
	return sendAction(std::make_unique<ActionHarkonnenRedraw>(mFaction, decision));
}

std::future<ResultCode> StandardPlayerClient::selectBeneGesseritStartArea(AreaId where)
{
	return sendAction(std::make_unique<ActionBeneGesseritStartingForce>(mFaction, where));
}

std::future<ResultCode> StandardPlayerClient::dialStorm(int dial)
{
	return sendAction(std::make_unique<ActionStormDial>(mFaction, dial));
}

std::future<ResultCode> StandardPlayerClient::decideChoamCharity(bool decision)
{
	return sendAction(std::make_unique<ActionChoamCharity>(mFaction, decision));
}

std::future<ResultCode> StandardPlayerClient::bid(BiddingAction action, int amount)
{
	return sendAction(std::make_unique<ActionBid>(mFaction, action, amount));
}

std::future<ResultCode> StandardPlayerClient::decideGuildTurn(bool decision)
{
	return sendAction(std::make_unique<ActionGuildShipmentDecision>(mFaction, decision));
}

std::future<ResultCode> StandardPlayerClient::shipFromReserve(Placement to)
{
	return sendAction(std::make_unique<ActionShip>(mFaction, to));
}

std::future<ResultCode> StandardPlayerClient::shipCrossPlanet(AreaId from, Placement to)
{
	return sendAction(std::make_unique<ActionShip>(mFaction, from, to));
}

std::future<ResultCode> StandardPlayerClient::shipToReserve(Placement from)
{
	return sendAction(std::make_unique<ActionShip>(mFaction, from, ActionShip::InvertShipment::TRUE));
}

std::future<ResultCode> StandardPlayerClient::reactToIntrusion(bool disengage)
{
	return sendAction(std::make_unique<ActionIntrusionReaction>(mFaction, disengage));
}

std::future<ResultCode> StandardPlayerClient::accompanyShipment(bool decision, bool toPolarSink)
{
	auto d = ActionAccompanyDecision::PASS;

	if (decision && toPolarSink)
		d = ActionAccompanyDecision::SEND_TO_POLAR_SINK;
	else if (decision)
		d = ActionAccompanyDecision::ACCOMPANY_SHIPMENT;

	return sendAction(std::make_unique<ActionAccompanyDecision>(mFaction, d));
}

std::future<ResultCode> StandardPlayerClient::move(AreaId aFrom, Placement aTo, bool aUseHajr)
{
	return sendAction(std::make_unique<ActionMove>(mFaction, aFrom, aTo, aUseHajr));
}

std::future<ResultCode> StandardPlayerClient::moveAsAdvisor(AreaId aFrom, Placement aTo, bool aUseHajr)
{
	return sendAction(std::make_unique<ActionMove>(mFaction, aFrom, aTo, ActionMove::AsAdvisor::TRUE, aUseHajr));
}

std::future<ResultCode> StandardPlayerClient::selectBattle(int id)
{
	return sendAction(std::make_unique<ActionBattleSelection>(mFaction, id));
}

std::future<ResultCode> StandardPlayerClient::commitBattlePlan(BattlePlan&& battlePlan)
{
	return sendAction(std::make_unique<ActionCommitBattlePlan>(mFaction, std::move(battlePlan)));
}

std::future<ResultCode> StandardPlayerClient::sendAction(std::unique_ptr<Action>&& action)
{
	return broker().sendConfirmed(mGameId, std::move(action));
}

void StandardPlayerClient::connect(bool asGameMaster)
{
	auto result = sendAction(std::make_unique<ActionConnect>(mFaction, asGameMaster));
	result.wait();

	if (result.get().isError())
		throw std::runtime_error("failed to connect client to the game");
}

void StandardPlayerClient::disconnect() noexcept
{
	try
	{
		sendAction(std::make_unique<ActionDisconnect>(mFaction));
	}
	catch (...) {}
}

ResultCode StandardPlayerClient::executeMessage(std::unique_ptr<Message>&& msg)
{
	return ResultCode();
}
