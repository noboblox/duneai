#pragma once
#include <climits>

class ResultCode
{
public:
	enum Value
	{
		SUCCESS = 0,
		UNKNOWN_MESSAGE,
		TARGET_NOT_REACHABLE,
		TARGET_SHUTDOWN,

		UNSPECIFIED_ERROR = INT_MAX
	};


	constexpr explicit ResultCode()
	: mValue(Value::SUCCESS)
	{
	}

	constexpr explicit ResultCode(Value aValue) 
		: mValue(aValue)
	{
	}
	
	inline bool isSuccess() const noexcept
	{
		return mValue == Value::SUCCESS;
	}

	inline bool isError() const noexcept 
	{
		return !isSuccess();
	}

private:
	Value mValue;
};
