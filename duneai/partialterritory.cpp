#include "partialterritory.h"

#include <algorithm>
#include <stdexcept>

PartialTerritory::PartialTerritory()
: mAreas{AreaId::INVALID, AreaId::INVALID, AreaId::INVALID, AreaId::INVALID, AreaId::INVALID}
{
}

PartialTerritory::PartialTerritory(AreaId a)
: mAreas{a, AreaId::INVALID, AreaId::INVALID, AreaId::INVALID, AreaId::INVALID}
{
}

PartialTerritory::PartialTerritory(AreaId a, AreaId b)
: mAreas{a, b, AreaId::INVALID, AreaId::INVALID, AreaId::INVALID}
{
}

PartialTerritory::PartialTerritory(AreaId a, AreaId b, AreaId c)
: mAreas{a, b, c, AreaId::INVALID, AreaId::INVALID}
{
}

PartialTerritory::PartialTerritory(AreaId a, AreaId b, AreaId c, AreaId d)
: mAreas{a, b, c, d, AreaId::INVALID}
{
}

PartialTerritory::PartialTerritory(AreaId a, AreaId b, AreaId c, AreaId d, AreaId e)
: mAreas{a, b, c, d, e}
{
}

bool PartialTerritory::containsAllOf(const PartialTerritory& other) const
{
	auto it = other.mAreas.cbegin();
	for (; it != other.mAreas.cend(); ++it)
	{
		if (!contains(*it))
			return false;
	}

	return true;
}

bool PartialTerritory::containsSomeOf(const PartialTerritory& other) const
{
	auto it = other.mAreas.cbegin();
	for (; it != other.mAreas.cend(); ++it)
	{
		if (contains(*it))
			return true;
	}

	return false;
}

bool PartialTerritory::contains(AreaId area) const
{
	return std::find(mAreas.cbegin(), mAreas.cend(), area) != mAreas.cend();
}

void PartialTerritory::add(AreaId area)
{
	if (mAreas[0] != AreaId::INVALID && !sameTerritory(mAreas[0], area))
		throw std::invalid_argument("cannot add areas of different territories");

	AreaId* free = nullptr;

	for (int i = 0; i < mAreas.size(); ++i)
	{
		if (mAreas[i] == area)
			return;

		if (!free && mAreas[i] == AreaId::INVALID)
			free = mAreas.data() + i;
	}

	if (!free)
		throw std::invalid_argument("PartialTerritory does not support more than 5 areas inside one territory");

	*free = area;
}

void PartialTerritory::forEach(std::function<void(AreaId)> f) const
{
	std::for_each(mAreas.cbegin(), mAreas.cend(), [&](AreaId id) {
		if (id != AreaId::INVALID)
			f(id);
	});
}

PartialTerritory& PartialTerritory::merge(const PartialTerritory& other)
{
	other.forEach([this](AreaId area){
		add(area);
	});

	return *this;
}
