#include "messagethread.h"
#include <thread>

MessageThread::MessageThread(Broker& broker)
: Actor(broker),
  mMessageThread([this]() { this->messageLoop(); })
{
}


MessageThread::~MessageThread() noexcept
{
	shutdown();
}

void MessageThread::post(std::unique_ptr<Message>&& msg) {
	if (msg == nullptr)
		return;

	std::lock_guard<std::mutex> l(mPendingMutex);
	mPending.push_back(std::move(msg));
}

void MessageThread::messageLoop()
{
	while (!mNeedStop)
	{
		auto p_msg = popMessage();

		if (p_msg)
		{
			auto request = p_msg->requestId();
			auto result = executeMessage(std::move(p_msg));
			finishRequest(request, result);
		}
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void MessageThread::shutdown() noexcept
{
	mNeedStop = true;
	mMessageThread.join();
}

std::unique_ptr<Message> MessageThread::popMessage()
{
	std::lock_guard<std::mutex> l(mPendingMutex);

	if (mPending.empty())
		return nullptr;

	auto p_msg = std::move(mPending.front());
	mPending.erase(mPending.begin());

	return std::move(p_msg);
}
