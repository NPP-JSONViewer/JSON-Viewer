#pragma once
#include <string>

enum class JsonNodeType : short
{
    UNKNOWN,
    STRING,
    NUMBER,
    BOOL,
    ARRAY,
    OBJECT,
};

struct JsonNode
{
    std::string  key;
    std::string  value;
    JsonNodeType type = JsonNodeType::UNKNOWN;
};
