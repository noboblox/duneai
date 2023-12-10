#include "partialterritory.h"

#include <algorithm>
#include <stdexcept>

#include "stormposition.h"
#include "territories.h"

PartialTerritory::PartialTerritory() noexcept
: mAreas{}
{
}

PartialTerritory::PartialTerritory(AreaId a)
: mAreas{a}
{
	assertValidAreas();
}

PartialTerritory::PartialTerritory(AreaId a, AreaId b)
: mAreas{a, b}
{
	assertValidAreas();
}

PartialTerritory::PartialTerritory(AreaId a, AreaId b, AreaId c)
: mAreas{a, b, c}
{
	assertValidAreas();
}

PartialTerritory::PartialTerritory(AreaId a, AreaId b, AreaId c, AreaId d)
: mAreas{a, b, c, d}
{
	assertValidAreas();
}

PartialTerritory::PartialTerritory(AreaId a, AreaId b, AreaId c, AreaId d, AreaId e)
: mAreas{a, b, c, d, e}
{
	assertValidAreas();
}

bool PartialTerritory::containsAllOf(const PartialTerritory& other) const
{
	for (const auto area : other.mAreas)
	{
		if (!contains(area))
			return false;
	}

	return true;
}

bool PartialTerritory::contains(AreaId area) const
{
	return std::find(mAreas.cbegin(), mAreas.cend(), area) != mAreas.cend();
}

void PartialTerritory::add(AreaId area)
{
	if (area == AreaId::INVALID)
		throw std::invalid_argument("cannot add invalid areas");

	if (!mAreas.empty() && !sameTerritory(mAreas[0], area))
		throw std::invalid_argument("cannot add areas of different territories");

	mAreas.push_back(area);
}

void PartialTerritory::forEach(std::function<void(AreaId)> f) const
{
	std::for_each(mAreas.cbegin(), mAreas.cend(), [&](AreaId id) {
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

std::pair<PartialTerritory, PartialTerritory> PartialTerritory::divideBy(StormPosition storm) const
{
	auto it = std::find_if(mAreas.cbegin(), mAreas.cend(), [&](AreaId id) {
			return storm == StormPosition(Territories::sectorOf(id));
	});

	if (it == mAreas.cend())
		return std::make_pair(*this, PartialTerritory());

	PartialTerritory before, after;

	std::for_each(mAreas.cbegin(), mAreas.cend(), [&](AreaId id) {
		auto pos = StormPosition(Territories::sectorOf(id));

		if (pos < storm)
			before.add(id);
		else if (pos > storm)
			after.add(id);
	});

	return std::make_pair(before, after);
}

bool PartialTerritory::overlaps(const PartialTerritory& other) const
{
	return PartialTerritory::overlaps(*this, other);
}

bool PartialTerritory::overlaps(const PartialTerritory& l, const PartialTerritory& r)
{
	for (const auto& area : l.mAreas)
	{
		if (r.contains(area))
			return true;
	}

	return false;
}

void PartialTerritory::assertValidAreas() const
{
	auto it = std::find(mAreas.cbegin(), mAreas.cend(), AreaId::INVALID);

	if (it != mAreas.cend())
		throw std::invalid_argument("cannot add invalid areas");
}

std::string PartialTerritory::print() const
{
	std::string result("{ ");

	for (const auto& area : mAreas)
	{
		result.append(EnumAreaId::label(area));
		result.append(", ");
	}

	result.append("}");
	return  result;
}
