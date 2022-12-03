#ifndef SYSTEMEVENTS_H_
#define SYSTEMEVENTS_H_

#include "event.h"
#include "savegame.h"
#include <functional>

class GameState;

class SystemEvent : public Event
{
public:
	enum SystemType
	{
		T_GAMESTATE_REQUEST,
		T_SAVE_GAME
	};

	SystemType type() const noexcept { return mType; }

protected:
	SystemEvent(SystemType aType)
	: Event(ET_SYSTEM),
	  mType(aType)
	{
	}

private:
	SystemType mType;
};

class SystemRequestGameState : public SystemEvent
{
public:
	explicit SystemRequestGameState(std::function<void(const GameState&)> target)
	: SystemEvent(T_GAMESTATE_REQUEST),
	  invokeTarget(target)
	{
	}

	const std::function<void(const GameState&)> invokeTarget;
};

class SystemRequestSaveGame : public SystemEvent
{
public:
	explicit SystemRequestSaveGame(std::function<void(const SaveGame&)> target)
	: SystemEvent(T_SAVE_GAME),
	  invokeTarget(target)
	{
	}

	const std::function<void(const SaveGame&)> invokeTarget;
};


#endif /* SYSTEMEVENTS_H_ */
