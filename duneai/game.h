#ifndef GAME_H_
#define GAME_H_

#include "messagethread.h"
#include "gamelogic.h"

class Game : public MessageThread
{
public:
	MACRO_DELETE_ALL_DEFAULTS(Game)
	explicit Game(Broker& broker, bool withDevActions = false);

protected:
	ResultCode executeMessage(std::unique_ptr<Message>&& msg) override;

private:
	GameLogic mLogic;
};

#endif
