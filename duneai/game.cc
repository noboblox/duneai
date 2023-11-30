#include "game.h"

Game::Game(Broker& broker, bool withDevActions)
: MessageThread(broker),
  mLogic(withDevActions)
{
}

ResultCode Game::executeMessage(std::unique_ptr<Message>&& msg)
{
	auto p_action = dynamic_cast<Action*> (msg.get());

	if (!p_action)
		return ResultCode(ResultCode::UNKNOWN_MESSAGE);

	msg.release();
	auto p_owner = std::unique_ptr<Action>(p_action);

	mLogic.post(std::move(p_owner));
	
	if (mLogic.executeOne())
		return ResultCode();
	else
		return ResultCode(ResultCode::UNSPECIFIED_ERROR);
}
