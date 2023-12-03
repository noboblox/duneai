#ifndef PARTIALTERRITORY_H_
#define PARTIALTERRITORY_H_

#include <array>
#include <functional>

#include "areaid.h"

class PartialTerritory
{
public:
	PartialTerritory();
	PartialTerritory(AreaId a);
	PartialTerritory(AreaId a, AreaId b);
	PartialTerritory(AreaId a, AreaId b, AreaId c);
	PartialTerritory(AreaId a, AreaId b, AreaId c, AreaId d);
	PartialTerritory(AreaId a, AreaId b, AreaId c, AreaId d, AreaId e);

	bool containsAllOf(const PartialTerritory& other) const;
	bool containsSomeOf(const PartialTerritory& other) const;
	bool contains(AreaId area) const;
	void add(AreaId area);

	void forEach(std::function<void(AreaId)> f) const;

	PartialTerritory& merge(const PartialTerritory& other);

private:
	std::array<AreaId, 5> mAreas;
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
