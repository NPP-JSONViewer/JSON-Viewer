#include "JsonHandler.h"

JsonHandler::JsonHandler(const ParseOptions &options)
    : m_parseOptions(options)
{
}

auto JsonHandler::GetCompressedJson(const std::string &jsonText) -> const Result
{
    rapidjson::StringBuffer                    sb;
    rapidjson::Writer<rapidjson::StringBuffer> handler(sb);

    return ParseJson(jsonText, sb, handler);
}

auto JsonHandler::FormatJson(const std::string &jsonText, LE le, LF lf, char indentChar, unsigned indentLen) -> const Result
{
    rapidjson::StringBuffer                          sb;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> handler(sb);
    handler.SetLineEnding(le);
    handler.SetFormatOptions(lf);
    handler.SetIndent(indentChar, indentLen);

    return ParseJson(jsonText, sb, handler);
}

auto JsonHandler::ValidateJson(const std::string &jsonText) -> const Result
{
    rapidjson::StringBuffer                    sb;
    rapidjson::Writer<rapidjson::StringBuffer> handler(sb);

    return ParseJson(jsonText, sb, handler);
}
