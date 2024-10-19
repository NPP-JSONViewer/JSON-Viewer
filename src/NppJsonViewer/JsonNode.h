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

struct Position
{
    size_t nLine {};
    size_t nColumn {};

    void clear()
    {
        nLine = nColumn = 0;
    }
};

struct JsonKey
{
    Position    pos {};
    std::string strKey;

    void clear()
    {
        pos.clear();
        strKey.clear();
    }
};

struct JsonNode
{
    JsonKey      key;
    std::string  value;
    JsonNodeType type = JsonNodeType::UNKNOWN;
};
