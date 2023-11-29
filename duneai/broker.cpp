#include "broker.h"

#include <algorithm>
#include "actor.h"
#include "message.h"


size_t Broker::getNextActorId() noexcept
{
	static size_t actor_id = 0;
	return ++actor_id;
}
size_t Broker::getNextRequestId() noexcept
{
	static size_t request_id = 0;
	return ++request_id;
}

size_t Broker::addActor(Actor& actor)
{
	std::lock_guard<std::mutex> l(mDataMutex);

	auto found = std::find_if(mActors.cbegin(), mActors.cend(), [&actor](const auto& elem) -> bool {
		return elem.second == &actor;
		});

	if (found != mActors.cend())
		throw std::invalid_argument("actor is already registered");

	auto id = getNextActorId();
	mActors.insert(std::make_pair(id, &actor));
	return id;
}

void Broker::removeActor(size_t id)
{
	std::lock_guard<std::mutex> l(mDataMutex);
	mActors.erase(id);

	auto it = mRequests.begin();

	while(it != mRequests.end())
	{
		auto& data = it->second;
		if (data.targetActor == id)
		{
			data.pending.set_value(ResultCode(ResultCode::TARGET_SHUTDOWN));
			it = mRequests.erase(it);
		}
		else
			++it;
	}
}

void Broker::sendUnconfirmed(size_t to, std::unique_ptr<Message>&& msg)
{
	if (to == 0 || msg == nullptr)
		throw std::invalid_argument("parameters must not be null");

	std::lock_guard<std::mutex> l(mDataMutex);

	const auto target = mActors.find(to);

	if (target == mActors.cend())
		return;

	target->second->post(std::move(msg));
}

std::future<ResultCode> Broker::sendConfirmed(size_t to, std::unique_ptr<Message>&& msg)
{
	if (to == 0 || msg == nullptr)
		throw std::invalid_argument("parameters must not be null");

	std::lock_guard<std::mutex> l(mDataMutex);

	const auto target = mActors.find(to);

	if (target == mActors.cend())
		return errorImmediately(ResultCode::TARGET_NOT_REACHABLE);

	const auto requestId = getNextRequestId();
	msg->mRequestId = requestId;

	auto insertResult = mRequests.emplace(requestId, RequestData{to, std::promise<ResultCode>() });
	auto future = insertResult.first->second.pending.get_future();

	target->second->post(std::move(msg));
	return future;
}

void Broker::handleReply(size_t requestId, ResultCode result)
{
	if (!requestId)
		return;

	std::lock_guard<std::mutex> l(mDataMutex);
	
	const auto caller = mRequests.find(requestId);

	if (caller == mRequests.cend())
		return;

	caller->second.pending.set_value(result);
	mRequests.erase(caller);
}

std::future<ResultCode> Broker::errorImmediately(ResultCode::Value error)
{
	auto task = std::promise<ResultCode>();
	auto result = task.get_future();
	task.set_value(ResultCode(error));
	return result;
}
