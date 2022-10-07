#include "JsonHandler.h"

namespace rj = rapidjson;


JsonHandler::JsonHandler(const ParseOptions &options)
    : m_parseOptions(options)
{
}

auto JsonHandler::GetCompressedJson(const std::string &jsonText) -> const Result
{
    rj::StringBuffer                                                                                sb;
    rj::Writer<rj::StringBuffer, rj::UTF8<>, rj::UTF8<>, rj::CrtAllocator, rj::kWriteNanAndInfFlag> handler(sb);

    return ParseJson<flgBaseWriter>(jsonText, sb, handler);
}

auto JsonHandler::FormatJson(const std::string &jsonText, LE le, LF lf, char indentChar, unsigned indentLen) -> const Result
{
    rj::StringBuffer                                                                                      sb;
    rj::PrettyWriter<rj::StringBuffer, rj::UTF8<>, rj::UTF8<>, rj::CrtAllocator, rj::kWriteNanAndInfFlag> handler(sb);
    handler.SetLineEnding(le);
    handler.SetFormatOptions(lf);
    handler.SetIndent(indentChar, indentLen);

    return ParseJson<flgBaseWriter>(jsonText, sb, handler);
}

auto JsonHandler::ValidateJson(const std::string &jsonText) -> const Result
{
    rj::StringBuffer                                                                                sb;
    rj::Writer<rj::StringBuffer, rj::UTF8<>, rj::UTF8<>, rj::CrtAllocator, rj::kWriteNanAndInfFlag> handler(sb);

    return ParseJson<flgBaseWriter>(jsonText, sb, handler);
}
