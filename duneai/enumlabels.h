#ifndef ENUMLABELS_H_
#define ENUMLABELS_H_

#include <unordered_map>

template <typename E>
class EnumLabels
{
public:
	static const char* label(E value)
	{
		auto it = labels.find(value);

		if (it == labels.cend())
			return "UNDEFINED";
		else
			return it->second;
	}
private:
	static std::unordered_map<E, const char*> labels;
};

#endif /* ENUMLABELS_H_ */
