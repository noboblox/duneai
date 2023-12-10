#ifndef PARTIALTERRITORY_H_
#define PARTIALTERRITORY_H_

#include <functional>
#include <string>
#include <vector>

#include "areaid.h"

class StormPosition;

class PartialTerritory
{
public:
	PartialTerritory() noexcept;
	PartialTerritory(AreaId a);
	PartialTerritory(AreaId a, AreaId b);
	PartialTerritory(AreaId a, AreaId b, AreaId c);
	PartialTerritory(AreaId a, AreaId b, AreaId c, AreaId d);
	PartialTerritory(AreaId a, AreaId b, AreaId c, AreaId d, AreaId e);

	bool containsAllOf(const PartialTerritory& other) const;
	bool containsSomeOf(const PartialTerritory& other) const { return overlaps(other); }
	bool contains(AreaId area) const;
	void add(AreaId area);

	void forEach(std::function<void(AreaId)> f) const;
	int count() const noexcept { return mAreas.size(); }
	bool empty() const noexcept { return mAreas.empty(); }

	PartialTerritory& merge(const PartialTerritory& other);

	std::pair<PartialTerritory, PartialTerritory> divideBy(StormPosition storm) const;
	bool overlaps(const PartialTerritory& other) const;

	static bool overlaps(const PartialTerritory& l, const PartialTerritory& r);
	std::string print() const;

private:
	void assertValidAreas() const;
	std::vector<AreaId> mAreas;
};

class Territory : public PartialTerritory
{
private:
	friend class Territories;

	Territory()	: PartialTerritory() {}
	Territory(AreaId a)	: PartialTerritory(a) {}
	Territory(AreaId a, AreaId b) : PartialTerritory(a, b) {}
	Territory(AreaId a, AreaId b, AreaId c) : PartialTerritory(a, b, c) {}
	Territory(AreaId a, AreaId b, AreaId c, AreaId d) : PartialTerritory(a, b, c, d) {}
	Territory(AreaId a, AreaId b, AreaId c, AreaId d, AreaId e) : PartialTerritory(a, b, c, d, e) {}
};


#endif /* PARTIALTERRITORY_H_ */
