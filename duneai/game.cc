#include "game.h"

Game::Game()
: MessageThread(),
  mLogic()
{
}

Game::Game(GameLogic&& logic)
: MessageThread(),
  mLogic(std::move(logic))
{
}

void Game::executeMessage(std::unique_ptr<Message>&& msg)
{
	auto p_action = dynamic_cast<Action*> (msg.get());

	if (!p_action)
		return;

	msg.release();
	auto p_owner = std::unique_ptr<Action>(p_action);

	mLogic.post(std::move(p_owner));
	mLogic.executeOne();
}