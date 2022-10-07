#pragma once
#include <string>

enum class JsonNodeType
{
    UNKNOWN = 0x00,
    STRING  = 0x01,
    NUMBER  = 0x02,
    BOOL    = 0x04,
    ARRAY   = 0x08,
    OBJECT  = 0x10
};

struct JsonNodePos
{
    int line = 0;
    int col  = 0;
};

struct JsonNode
{
    std::string  key;
    std::string  value;
    JsonNodeType type = JsonNodeType::UNKNOWN;
    JsonNodePos  pos  = {};
};
