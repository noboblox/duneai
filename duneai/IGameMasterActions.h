#ifndef IGAMEMASTERACTIONS_H_
#define IGAMEMASTERACTIONS_H_

#include <future>
#include "resultcode.h"

class IGameMasterActions
{
public:
	virtual ~IGameMasterActions() {}

	virtual std::future<ResultCode> startGame() = 0;
};

#endif
