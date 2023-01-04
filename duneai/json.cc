#include <memory>

#include "json.h"
#include "thirdparty/nlohmann/json.hpp"

class JsonImpl : public nlohmann::json
{
public:
    JsonImpl(nlohmann::json&& o)
    : nlohmann::json(std::move(o))
    {
    }
};

static void Assert(bool checked)
{
    if (!checked)
        throw std::invalid_argument("unexpected json format");
}

Json::Json(const std::string& source)
: mpDoc()
{
    auto begin = source.data();
    auto end   = begin + source.size();
    auto doc   = JsonImpl::parse(begin, end);
    mpDoc      = std::make_unique<JsonImpl>(std::move(doc));

    Assert(mpDoc->is_object());
}

Json::~Json()
{
}

std::string Json::getString(const char* key)
{
    auto it = mpDoc->find(key);

    Assert(it != mpDoc->end() && it->is_string());
    return it->get<std::string>();
}

int Json::getInt(const char* key)
{
    auto it = mpDoc->find(key);

    Assert(it != mpDoc->end() && it->is_number_integer());
    return it->get<int>();
}

ActionType Json::getActionType(const char* key)
{
    return EnumActionType::value(getString(key));
}

Faction Json::getExactlyOneFaction(const char* key)
{
    Faction result = Faction::fromString(getString(key));
    Assert(result.exactlyOne());
    return result;
}

Leader::Id Json::getLeaderId(const char* key)
{
    return EnumLeaderId::value(getString(key));
}
