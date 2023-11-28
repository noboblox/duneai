#pragma once

#include <future>
#include <memory>
#include <mutex>
#include <map>

#include "resultcode.h"
class Actor;
class Message;

class Broker
{
public:
	explicit Broker()
	{
	}

	~Broker()
	{
	}

	size_t addActor(Actor& actor);
	void removeActor(size_t id);

	std::future<ResultCode> sendConfirmed(size_t to, std::unique_ptr<Message>&& msg);
	void sendUnconfirmed(size_t to, std::unique_ptr<Message>&& msg);
	void handleReply(size_t requestId, ResultCode result);

private:
	static size_t getNextActorId() noexcept;
	static size_t getNextRequestId() noexcept;
	std::future<ResultCode> errorImmediately(ResultCode::Value error);

private:
	struct RequestData
	{
		size_t targetActor;
		std::promise<ResultCode> pending;
	};

	std::mutex mDataMutex;
	std::map <size_t, Actor*> mActors;
	std::map <size_t, RequestData> mRequests;
};

