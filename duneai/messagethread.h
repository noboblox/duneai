#ifndef MESSAGE_THREAD_H_
#define MESSAGE_THREAD_H_

#include <atomic>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "deletedefaults.h"
#include "message.h"

class MessageThread
{
public:
	MACRO_DELETE_ALL_DEFAULTS(MessageThread)
		
	explicit MessageThread()
	{
		auto thread = std::thread([this]() { this->messageLoop(); });
		thread.detach();
	}


	virtual ~MessageThread() noexcept
	{
		shutdown();
	}

	void post(std::unique_ptr<Message>&& msg) {
		if (msg == nullptr)
			return;

		std::lock_guard<std::mutex> l(mQueueMutex);
		mPending.push_back(std::move(msg));
	}

private:
	virtual void executeMessage(std::unique_ptr<Message>&& msg) = 0;

	void messageLoop()
	{
		while (!mNeedStop)
		{
			auto p_msg = popMessage();

			if (p_msg)
				executeMessage(std::move(p_msg));
			else
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}

	void shutdown() noexcept
	{
		mNeedStop = true;
	}

private:
	std::unique_ptr<Message> popMessage()
	{
		std::lock_guard<std::mutex> l(mQueueMutex);

		if (mPending.empty())
			return nullptr;

		auto p_msg = std::move(mPending.front());
		mPending.erase(mPending.begin());

		return std::move(p_msg);
	}

private:
	std::atomic<bool> mNeedStop = false;
	std::mutex mQueueMutex;
	std::vector<std::unique_ptr<Message>> mPending;
};

#endif // !MESSAGE_THREAD_H_
