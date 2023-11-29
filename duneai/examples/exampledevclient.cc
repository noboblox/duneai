#include "../devgame.h"
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

	DevGame game(broker);
	DevClient client(Faction::any(), broker, game);

	await(client.startWithoutDraw());


	return 0;
}
