#ifndef ENUMLABELS_H_
#define ENUMLABELS_H_

#include <vector>
#include <utility>
#include <string>

template <typename E, E invalidValue>
class Enum
{
public:
	using Definition = std::vector<std::pair<E, const char*>>;

	static const char* label(E value)
	{
		auto it = std::find_if(entries.cbegin(), entries.cend(),
				[value](const auto& e) { return e.first == value; });

		if (it == entries.cend())
			return "UNDEFINED";
		else
			return it->second;
	}

	static E value(const std::string& label)
	{
		auto it = std::find_if(entries.cbegin(), entries.cend(),
			[label](const auto& e) { return label == e.second; });

		if (it == entries.cend())
			return invalidValue;
		else
			return it->first;
	}

private:
	static Definition entries;
};

#endif /* ENUMLABELS_H_ */
