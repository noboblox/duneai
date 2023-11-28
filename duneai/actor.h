#pragma once

#include <memory>

#include "deletedefaults.h"
#include "resultcode.h"
class Broker;
class Message;

class Actor
{
public:
	MACRO_DELETE_ALL_DEFAULTS(Actor)

	explicit Actor(Broker& broker);
	const size_t actorId() const noexcept;
	void finishRequest(size_t requestId, ResultCode result);

	virtual void post(std::unique_ptr<Message>&& msg) = 0;
	
	virtual ~Actor();

protected:
	Broker& broker() noexcept;

private:
	Broker& mBroker;
	const size_t mActorId;
};

