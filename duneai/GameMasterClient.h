#ifndef GAMEMASTERCLIENT_H_
#define GAMEMASTERCLIENT_H_

#include "standardplayerclient.h"
#include "IGameMasterActions.h"

class GameMasterClient : public StandardPlayerClient, IGameMasterActions
{
public:
	explicit GameMasterClient(Faction own, Broker& broker, const Game& game);

	std::future<ResultCode> startGame() override;
};

#endif
