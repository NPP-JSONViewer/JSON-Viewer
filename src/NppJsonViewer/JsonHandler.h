#pragma once

#include <string>

#include <rapidjson/reader.h>
#include <rapidjson/writer.h>
#include "rapidjson/document.h"
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/error/en.h>
#include "Define.h"

namespace rj = rapidjson;

struct Result
{
    bool        success    = false;
    int         error_pos  = -1;
    int         error_code = -1;
    std::string error_str;
    std::string response;
};

using LE = rj::LineEndingOption;
using LF = rj::PrettyFormatOptions;

constexpr auto flgBaseReader = rj::kParseEscapedApostropheFlag | rj::kParseNanAndInfFlag | rj::kParseNumbersAsStringsFlag;
constexpr auto flgBaseWriter = rj::kParseEscapedApostropheFlag | rj::kParseNanAndInfFlag | rj::kParseFullPrecisionFlag;

class JsonHandler
{
    ParseOptions m_parseOptions {};

public:
    explicit JsonHandler(const ParseOptions &options);
    ~JsonHandler() = default;

    auto GetCompressedJson(const std::string &jsonText) -> const Result;
    auto FormatJson(const std::string &jsonText, LE le, LF lf, char indentChar, unsigned indentLen) -> const Result;
    auto SortJsonByKey(const std::string &jsonText, LE le, LF lf, char indentChar, unsigned indentLen) -> const Result;
    auto ValidateJson(const std::string &jsonText) -> const Result;

    template <unsigned format, typename Handler>
    auto ParseJson(const std::string &jsonText, rj::StringBuffer &sb, Handler &handler) -> const Result;

private:
    void SortJsonObject(rj::Value &jsonObject, rj::Document::AllocatorType &allocator) const;
    void SortJsonRecursively(rj::Value &jsonValue, rj::Document::AllocatorType &allocator) const;
    auto SortJsonText(const std::string &jsonString) const -> std::string;
};

template <unsigned flgBase, typename Handler>
inline auto JsonHandler::ParseJson(const std::string &jsonText, rj::StringBuffer &sb, Handler &handler) -> const Result
{
    Result retVal {};

    bool             success = false;
    rj::Reader       reader;
    rj::StringStream ss(jsonText.c_str());

    // TODO: Find some better way
    constexpr auto flgBase_comment = flgBase | rj::kParseCommentsFlag;
    constexpr auto flgBase_comma   = flgBase | rj::kParseTrailingCommasFlag;
    constexpr auto flgBase_Both    = flgBase_comma | flgBase_comment;

    if (m_parseOptions.bIgnoreComment && m_parseOptions.bIgnoreTrailingComma)
    {
        success = reader.Parse<flgBase_Both>(ss, handler) && sb.GetString();
    }

    else if (!m_parseOptions.bIgnoreComment && m_parseOptions.bIgnoreTrailingComma)
    {
        success = reader.Parse<flgBase_comma>(ss, handler) && sb.GetString();
    }

    else if (m_parseOptions.bIgnoreComment && !m_parseOptions.bIgnoreTrailingComma)
    {
        success = reader.Parse<flgBase_comment>(ss, handler) && sb.GetString();
    }

    else if (!m_parseOptions.bIgnoreComment && !m_parseOptions.bIgnoreTrailingComma)
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
        retVal.error_str  = rj::GetParseError_En(reader.GetParseErrorCode());
        retVal.error_pos  = static_cast<int>(reader.GetErrorOffset());
        retVal.error_code = reader.GetParseErrorCode();
        retVal.response.clear();
    }

    return retVal;
}
