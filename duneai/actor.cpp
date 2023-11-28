#include "actor.h"
#include "broker.h"
#include "message.h"

Actor::Actor(Broker& broker)
	: mBroker(broker),
	  mActorId(broker.addActor(*this))
{
}
Actor::~Actor()
{
	mBroker.removeActor(mActorId);
}

const size_t Actor::actorId() const noexcept
{
	return mActorId;
}

void Actor::finishRequest(size_t requestId, ResultCode result)
{
	mBroker.handleReply(requestId, result);
}

Broker& Actor::broker() noexcept
{
	return mBroker;
}
