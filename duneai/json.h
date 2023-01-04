#ifndef JSON_H_
#define JSON_H_

#include <memory>
#include "actions.h"

class JsonImpl;

class Json
{
public:
    explicit Json(const std::string& source);
    ~Json();

    std::string getString(const char* key);
    int getInt(const char* key);
    ActionType getActionType(const char* key);
    Faction getExactlyOneFaction(const char* key);

private:
    std::unique_ptr<JsonImpl> mpDoc;
};

#endif
