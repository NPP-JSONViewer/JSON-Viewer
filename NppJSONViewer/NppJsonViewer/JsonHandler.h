#pragma once

#include <string>

#include <rapidjson/reader.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/error/en.h>
#include "Define.h"

struct Result
{
    bool        success    = false;
    int         error_pos  = -1;
    int         error_code = -1;
    std::string error_str;
    std::string response;
};

using LE = rapidjson::LineEndingOption;
using LF = rapidjson::PrettyFormatOptions;

class JsonHandler
{
    ParseOptions m_parseOptions {};

public:
    JsonHandler(const ParseOptions &options);
    ~JsonHandler() = default;

    auto GetCompressedJson(const std::string &jsonText) -> const Result;
    auto FormatJson(const std::string &jsonText, LE le, LF lf, char indentChar, unsigned indentLen) -> const Result;
    auto ValidateJson(const std::string &jsonText) -> const Result;

    template <typename Handler>
    auto ParseJson(const std::string &jsonText, rapidjson::StringBuffer &sb, Handler &handler) -> const Result;
};

template <typename Handler>
inline auto JsonHandler::ParseJson(const std::string &jsonText, rapidjson::StringBuffer &sb, Handler &handler) -> const Result
{
    Result retVal {};

    bool                    success = false;
    rapidjson::Reader       reader;
    rapidjson::StringStream ss(jsonText.c_str());

    // TODO: Find some better way
    constexpr auto flgBase         = rapidjson::kParseFullPrecisionFlag | rapidjson::kParseEscapedApostropheFlag | rapidjson::kParseNanAndInfFlag;
    constexpr auto flgBase_commemt = flgBase | rapidjson::kParseCommentsFlag;
    constexpr auto flgBase_comma   = flgBase | rapidjson::kParseTrailingCommasFlag;
    constexpr auto flgBase_Both    = flgBase_comma | flgBase_commemt;

    if (m_parseOptions.bIgnoreComment && m_parseOptions.bIgnoreTraillingComma)
    {
        success = reader.Parse<flgBase_Both>(ss, handler) && sb.GetString();
    }

    else if (!m_parseOptions.bIgnoreComment && m_parseOptions.bIgnoreTraillingComma)
    {
        success = reader.Parse<flgBase_comma>(ss, handler) && sb.GetString();
    }

    else if (m_parseOptions.bIgnoreComment && !m_parseOptions.bIgnoreTraillingComma)
    {
        success = reader.Parse<flgBase_commemt>(ss, handler) && sb.GetString();
    }

    else if (!m_parseOptions.bIgnoreComment && !m_parseOptions.bIgnoreTraillingComma)
    {
        success = reader.Parse<flgBase>(ss, handler) && sb.GetString();
    }

    if (success)
    {
        retVal.success    = true;
        retVal.response   = sb.GetString();
        retVal.error_code = retVal.error_pos = -1;
        retVal.error_str.clear();
    }
    else
    {
        retVal.success    = false;
        retVal.error_str  = rapidjson::GetParseError_En(reader.GetParseErrorCode());
        retVal.error_pos  = static_cast<int>(reader.GetErrorOffset());
        retVal.error_code = reader.GetParseErrorCode();
        retVal.response.clear();
    }

    return retVal;
}
