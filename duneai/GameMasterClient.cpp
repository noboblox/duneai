#include "GameMasterClient.h"
#include "actions.h"

GameMasterClient::GameMasterClient(Faction own, Broker& broker, const Game& game)
: StandardPlayerClient(own, broker, game, true)
{
}

std::future<ResultCode> GameMasterClient::startGame()
{
	return sendAction(std::make_unique<GmActionStartGame>(faction()));
}
