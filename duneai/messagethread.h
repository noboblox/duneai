#ifndef MESSAGE_THREAD_H_
#define MESSAGE_THREAD_H_

#include "actor.h"

#include <atomic>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "deletedefaults.h"
#include "message.h"
#include "resultcode.h"

class MessageThread : public Actor
{
public:
	explicit MessageThread(Broker& broker);
	virtual ~MessageThread() noexcept;

	void post(std::unique_ptr<Message>&& msg) override;

protected:
	virtual ResultCode executeMessage(std::unique_ptr<Message>&& msg) = 0;

private:
	void messageLoop();
	void shutdown() noexcept;
	std::unique_ptr<Message> popMessage();

private:
	std::atomic<bool> mNeedStop = false;
	std::mutex mPendingMutex;
	std::vector<std::unique_ptr<Message>> mPending;
	std::thread mMessageThread;
};

#endif // !MESSAGE_THREAD_H_
