#include "../devgame.h"
#include "../broker.h"
#include "../forces.h"
#include "../standardplayerclient.h"
class DevClient
{
public:
	explicit DevClient(Faction factions, Broker& broker, const Game& game)
	{
		auto neededFactions = Faction::expand(factions);

		std::for_each(neededFactions.cbegin(), neededFactions.cend(), [&](const Faction& f){
			mClients.emplace(f, std::make_unique<StandardPlayerClient>(f, broker, game));
			});
	}

	StandardPlayerClient& as(Faction faction)
	{
		auto found = mClients.find(faction);

		if (found == mClients.cend())
			throw std::invalid_argument("faction is not available");

		return *found->second;
	}
private:
	std::map<Faction, std::unique_ptr<StandardPlayerClient>> mClients;
};


int main()
{
	Broker broker;

	DevGame game(broker);
	DevClient client(Faction::any(), broker, game);

	auto result = client.as(Faction::beneGesserit()).predictWinner(Faction::fremen(), 3);
	result.wait();
	auto r = result.get();

	return 0;
}
