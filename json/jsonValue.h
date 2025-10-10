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

    [[nodiscard]] std::string typeToString() const;
    [[nodiscard]] std::string valueToString() const;

    operator double() const {
        return std::get<double>(value);
    }

    operator std::string() const {
        return std::get<std::string>(value);
    }

    operator bool() const {
        return std::get<bool>(value);
    }
};