#pragma once

class IDevActions
{
public:
	virtual ~IDevActions() {}

	virtual std::future<ResultCode> startWithoutDraw() = 0;
};
