#ifndef MESSAGE_THREAD_H_
#define MESSAGE_THREAD_H_

#include <atomic>
#include <memory>
#include <mutex>
#include <vector>

#include "deletedefaults.h"
#include "message.h"

class MessageThread
{
public:
	MACRO_DELETE_ALL_DEFAULTS(MessageThread)

	explicit MessageThread();
	virtual ~MessageThread() noexcept;

	void post(std::unique_ptr<Message>&& msg);

private:
	virtual void executeMessage(std::unique_ptr<Message>&& msg) = 0;

	void messageLoop();
	void shutdown() noexcept;
	std::unique_ptr<Message> popMessage();

private:
	std::atomic<bool> mNeedStop = false;
	std::mutex mPendingMutex;
	std::vector<std::unique_ptr<Message>> mPending;
};

#endif // !MESSAGE_THREAD_H_
