#pragma once

class Message
{
public:
	friend class Broker;

	virtual ~Message() {};

	const size_t requestId()
	{
		return mRequestId;
	}

private:
	size_t mRequestId = 0;
};