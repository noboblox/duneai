#ifndef STORMPOSITION_H_
#define STORMPOSITION_H_

#include <stdexcept>

class StormPosition
{
public:
	explicit StormPosition() noexcept
	: StormPosition(1)
	{
	}

	explicit StormPosition(int position)
	: mPosition(position >= 0 ? guardRange(position) : 0)
	{
	}

	int advanceBy(int value) noexcept
	{
		int newPosition = mPosition + value >= 0 ? value : 0;
		mPosition = guardRange(newPosition);
		return mPosition;
	}

	bool valid() const noexcept { return mPosition != 0; }

	int value() const noexcept { return mPosition; }

	bool operator!=(StormPosition other) const noexcept { return !operator==(other); }
	bool operator>(StormPosition other) const noexcept  { return other.operator<(*this); }
	bool operator>=(StormPosition other) const noexcept { return !operator<(other); }
	bool operator<=(StormPosition other) const noexcept { return other.operator>=(*this); }

	bool operator==(StormPosition other) const noexcept
	{
		if (!valid() || !other.valid())
			return false;

		return mPosition == other.mPosition;
	}

	bool operator<(StormPosition other) const noexcept
	{
		if (!valid() || !other.valid())
			return false;

		int distance = advanceDistance(other);
		return distance > 0 && distance <= AFTER_STORM_MAX_DISTANCE;
	}

	int advanceDistance(StormPosition other) const noexcept
	{
		StormPosition copy(*this);
		int distance = 0;

		while (copy != other)
		{
			copy.advanceBy(1);
			++distance;
		}

		return distance;
	}

private:
	static constexpr int AFTER_STORM_MAX_DISTANCE = 9;

	int guardRange(int raw) { return ((raw - 1) % 18) + 1; }
	int mPosition;
};


#endif /* STORMPOSITION_H_ */
