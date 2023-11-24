#ifndef GAME_H_
#define GAME_H_

#include "messagethread.h"
#include "gamelogic.h"



class Game : public MessageThread
{
public:
	MACRO_DELETE_ALL_DEFAULTS(Game)

	explicit Game()	: MessageThread()
	{
	}

	void executeMessage(std::unique_ptr<Message>&& msg) override
	{
		// TODO!
		std::unique_ptr<Action> p = nullptr;

		mLogic.post(std::move(p));
		mLogic.executeOne();
	}

private:
	GameLogic mLogic = GameLogic();
};

#endif
