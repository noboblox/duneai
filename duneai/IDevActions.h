#pragma once

class IDevActions
{
public:
	virtual ~IDevActions() {}

	virtual std::future<ResultCode> startCustom(StartActions actions, int seed) = 0;
	virtual std::future<ResultCode> setGamePhase(GamePhase phase) = 0;
	virtual std::future<ResultCode> setStorm(int position) = 0;
	virtual std::future<ResultCode> setForces(Faction who, Placement what, bool isHostile = true) = 0;
};
