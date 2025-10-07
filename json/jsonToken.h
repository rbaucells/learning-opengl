#pragma once
#include <string>

struct JsonToken {
    enum class Type {
        start_object,//
        end_object,//
        start_array,//
        end_array,//
        comma, //
        colon, //

        null, //
        boolean, //
        string, //
        number, //
        eof,
    };

    Type type = Type::null;
    std::u8string value;
};
