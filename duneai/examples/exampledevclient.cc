#include "../game.h"
#include "../broker.h"
#include "../forces.h"
#include "../GameMasterClient.h"
#include "../IDevActions.h"

class DevClient : public IDevActions
{
public:
	explicit DevClient(Faction factions, Broker& broker, const Game& game)
	: mBroker(broker),
	  mGame(game.actorId())
	{
		auto neededFactions = Faction::expand(factions);

		std::for_each(neededFactions.cbegin(), neededFactions.cend(), [&](const Faction& f){
			mClients.emplace(f, std::make_unique<GameMasterClient>(f, broker, game));
			});
	}

	GameMasterClient& as(Faction faction)
	{
		auto found = mClients.find(faction);

		if (found == mClients.cend())
			throw std::invalid_argument("faction is not available");

		return *found->second;
	}

	GameMasterClient& asAny()
	{
		return *mClients.begin()->second;
	}

	std::future<ResultCode> startWithoutDraw() override
	{
		return mBroker.sendConfirmed(mGame, std::make_unique<DevActionStartWithoutDraw>());
	}

	std::future<ResultCode> setStorm(int position) override
	{
		return mBroker.sendConfirmed(mGame, std::make_unique<DevActionSetStorm>(position));
	}


	std::future<ResultCode> setGamePhase(GamePhase phase) override
	{
		return mBroker.sendConfirmed(mGame, std::make_unique<DevActionSetPhase>(phase));
	}

	std::future<ResultCode> setForces(Faction who, Placement what, bool isHostile = true) override
	{
		return mBroker.sendConfirmed(mGame, std::make_unique<DevActionPlaceTroops>(who, what, isHostile));
	}

private:
	std::map<Faction, std::unique_ptr<GameMasterClient>> mClients;
	Broker& mBroker;
	size_t mGame;
};

static void await(std::future<ResultCode>&& task)
{
	task.wait();
	if (task.get().isError())
		throw std::runtime_error("awaited task failed");
}


int main()
{
	Broker broker;

	Game game(broker, true);
	DevClient client(Faction::any(), broker, game);

	await(client.startWithoutDraw());
	// TODO do not shuffle seats in test scenarios
	await(client.setStorm(6));

	await(client.setForces(Faction::atreides(), Placement{AreaId::FalseWallEast_5, 4, 0}));
	await(client.setForces(Faction::atreides(), Placement{AreaId::FalseWallEast_7, 2, 0}));
	await(client.setForces(Faction::atreides(), Placement{AreaId::FalseWallEast_9, 2, 0}));

	await(client.setForces(Faction::beneGesserit(), Placement{AreaId::FalseWallEast_5, 2, 0}, false));
	await(client.setForces(Faction::harkonnen(), Placement{AreaId::FalseWallEast_5, 2, 0}));
	await(client.setForces(Faction::fremen(), Placement{AreaId::FalseWallEast_8, 2, 1}));
	await(client.setForces(Faction::emperor(), Placement{AreaId::FalseWallEast_8, 2, 1}));
	await(client.setGamePhase(GamePhase::PHASE_BATTLE_COLLECT_BATTLES));

	return 0;
}
