#ifndef SYSTEMEVENTS_H_
#define SYSTEMEVENTS_H_

#include "event.h"
#include <functional>

class GameState;

class SystemEvent : public Event
{
public:
	enum SystemType
	{
		T_GAMESTATE_REQUEST
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
	  invoceTarget(target)
	{
	}

	const std::function<void(const GameState&)> invoceTarget;
};


#endif /* SYSTEMEVENTS_H_ */
