#pragma once

class IDevActions
{
public:
	virtual ~IDevActions() {}

	virtual std::future<ResultCode> startWithoutDraw() = 0;
	virtual std::future<ResultCode> setGamePhase(GamePhase phase) = 0;
	virtual std::future<ResultCode> setStorm(int position) = 0;
};
