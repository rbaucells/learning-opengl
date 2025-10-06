#pragma once
#include <string>

#include "jsonArray.h"
#include "jsonObject.h"

struct JsonValue {
    enum class Type {
        null,
        boolean,
        string,
        number,
        object,
        array
    };

    Type type = Type::null;

    std::variant<std::monostate, bool, std::string, double, JsonObject, JsonArray> value;
};
