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
		auto p_action = dynamic_cast<Action*> (msg.get());

		if (!p_action)
			return;

		msg.release();
		auto p_owner = std::unique_ptr<Action>(p_action);

		mLogic.post(std::move(p_owner));
		mLogic.executeOne();
	}

private:
	GameLogic mLogic = GameLogic();
};

#endif
