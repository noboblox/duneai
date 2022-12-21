#include "forces.h"
#include "arrakis.h"
#include <algorithm>

bool Conflict::isValid() const noexcept
{
	return mParties.count() >= 2;
}

void Conflict::add(const ForcesFrom& participant)
{
	mForces.push_back(participant);
	mParties |= participant.from;
}

void Conflict::partition(Conflict source, int storm, std::vector<Conflict>& addTo)
{
	Conflict left, right;

	source.removeStormArea(storm);

	std::for_each(source.mForces.cbegin(), source.mForces.cend(),
			[&left, &right, &storm](const auto& f)
			{
				Arrakis::compareSector(Arrakis::areaSector(f.where), storm) > 0 ? left.add(f) : right.add(f);
			});

	if (left.isValid())
		addTo.push_back(left);
	if (right.isValid())
		addTo.push_back(right);
}

void Conflict::removeStormArea(int storm)
{
	auto it = std::find_if(mForces.begin(), mForces.end(),
				[storm, this](const auto& f) {	return Arrakis::areaSector(f.where) == storm; });

	if (it != mForces.end())
	{
		mForces.erase(it);
		updateParties();
	}
}

void Conflict::updateParties() noexcept
{
	mParties = Faction::none();
	std::for_each(mForces.cbegin(), mForces.cend(),
			[this](const auto& f) { mParties |= f.from; });
}



