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
    int nLine {};
    int nColumn {};
    void clear()
    {
        nLine = nColumn = 0;
    }
};

struct JsonLastKey
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
    JsonLastKey  key;
    std::string  value;
    JsonNodeType type = JsonNodeType::UNKNOWN;
};
