#pragma once

class ResultCode
{
public:
	enum Value
	{
		SUCCESS = 0,
		NOT_IMPLEMENTED = 999999,
	};

	explicit ResultCode()
	: mValue(Value::SUCCESS)
	{
	}

	explicit ResultCode(Value aValue)
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