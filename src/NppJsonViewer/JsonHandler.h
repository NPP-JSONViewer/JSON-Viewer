#pragma once

#include <expected>
#include <string>

#include <rapidjson/reader.h>
#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/error/en.h>

#include "Define.h"
#include "TrackingStream.h"

namespace rj = rapidjson;

struct JsonError
{
    int         position;
    int         code;
    std::string message;
};

using Result = std::expected<std::string, JsonError>;

using LE = rj::LineEndingOption;
using LF = rj::PrettyFormatOptions;

// Parse flags for all JSON operations. Historically flgBaseWriter used kParseFullPrecisionFlag
// instead of kParseNumbersAsStringsFlag because upstream RapidJSON's RawNumber() would quote
// numbers as strings. Our fork fixes that (RawNumber now writes via WriteRawValue without quotes),
// so both flag sets are now identical. They are kept separate to allow independent tuning of the
// DOM-based path (flgBaseReader: sort-by-key) vs the SAX streaming path (flgBaseWriter: format/compress)
// if needed in the future.
constexpr auto flgBaseReader = rj::kParseEscapedApostropheFlag | rj::kParseNanAndInfFlag | rj::kParseNumbersAsStringsFlag;
constexpr auto flgBaseWriter = rj::kParseEscapedApostropheFlag | rj::kParseNanAndInfFlag | rj::kParseNumbersAsStringsFlag;

class JsonHandler
{
    ParseOptions m_parseOptions {};

public:
    explicit JsonHandler(const ParseOptions& options);
    ~JsonHandler() = default;

    auto GetCompressedJson(const std::string& jsonText) -> Result;
    auto FormatJson(const std::string& jsonText, LE le, LF lf, char indentChar, unsigned indentLen) -> Result;
    auto FormatJsonLines(const std::string& jsonText, LE le, LF lf, char indentChar, unsigned indentLen) -> Result;
    auto SortJsonByKey(const std::string& jsonText, LE le, LF lf, char indentChar, unsigned indentLen) -> Result;
    auto ValidateJson(const std::string& jsonText) -> Result;

    template <unsigned format, typename Handler>
    auto ParseJson(const std::string& jsonText, rj::StringBuffer& sb, Handler& handler, TrackingStreamSharedPtr pTS = nullptr) -> Result;

private:
    void SortJsonObject(rj::Value& jsonObject, rj::Document::AllocatorType& allocator) const;
    void SortJsonRecursively(rj::Value& jsonValue, rj::Document::AllocatorType& allocator) const;
    auto SortJsonText(const std::string& jsonString) const -> std::string;
};

template <unsigned flgBase, typename Handler>
inline auto JsonHandler::ParseJson(const std::string& jsonText, rj::StringBuffer& sb, Handler& handler, TrackingStreamSharedPtr pTS) -> Result
{
    bool       success = false;
    rj::Reader reader;

    std::shared_ptr<rj::StringStream> pSS = nullptr;
    if (!pTS)
    {
        pSS = std::make_shared<rj::StringStream>(jsonText.c_str());
    }

    // TODO: Find some better way
    constexpr auto flgBase_comment = flgBase | rj::kParseCommentsFlag;
    constexpr auto flgBase_comma   = flgBase | rj::kParseTrailingCommasFlag;
    constexpr auto flgBase_Both    = flgBase_comma | flgBase_comment;

    if (m_parseOptions.bIgnoreComment && m_parseOptions.bIgnoreTrailingComma)
    {
        success = pTS ? reader.Parse<flgBase_Both>(*pTS, handler) && sb.GetString() : reader.Parse<flgBase_Both>(*pSS, handler) && sb.GetString();
    }

    else if (!m_parseOptions.bIgnoreComment && m_parseOptions.bIgnoreTrailingComma)
    {
        success = pTS ? reader.Parse<flgBase_comma>(*pTS, handler) && sb.GetString() : reader.Parse<flgBase_comma>(*pSS, handler) && sb.GetString();
    }

    else if (m_parseOptions.bIgnoreComment && !m_parseOptions.bIgnoreTrailingComma)
    {
        success = pTS ? reader.Parse<flgBase_comment>(*pTS, handler) && sb.GetString() : reader.Parse<flgBase_comment>(*pSS, handler) && sb.GetString();
    }

    else if (!m_parseOptions.bIgnoreComment && !m_parseOptions.bIgnoreTrailingComma)
    {
        success = pTS ? reader.Parse<flgBase>(*pTS, handler) && sb.GetString() : reader.Parse<flgBase>(*pSS, handler) && sb.GetString();
    }

    if (success)
    {
        return std::string(sb.GetString());
    }

    return std::unexpected(
        JsonError {
            .position = static_cast<int>(reader.GetErrorOffset()),
            .code     = static_cast<int>(reader.GetParseErrorCode()),
            .message  = rj::GetParseError_En(reader.GetParseErrorCode())});
}
