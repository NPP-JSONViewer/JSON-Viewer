#include <vector>
#include <algorithm>

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

auto JsonHandler::SortJsonByKey(const std::string &jsonText, LE le, LF lf, char indentChar, unsigned indentLen) -> const Result
{
    auto res = ValidateJson(jsonText);
    if (res.success)
    {
        // Sort the JSON string
        auto sorted = SortJsonText(jsonText);
        res         = FormatJson(sorted, le, lf, indentChar, indentLen);
    }
    return res;
}

auto JsonHandler::ValidateJson(const std::string &jsonText) -> const Result
{
    rj::StringBuffer                                                                                sb;
    rj::Writer<rj::StringBuffer, rj::UTF8<>, rj::UTF8<>, rj::CrtAllocator, rj::kWriteNanAndInfFlag> handler(sb);

    return ParseJson<flgBaseWriter>(jsonText, sb, handler);
}

void JsonHandler::SortJsonObject(rj::Value &jsonObject, rj::Document::AllocatorType &allocator) const
{
    if (!jsonObject.IsObject())
    {
        return;
    }

    std::vector<std::string> keys;

    // Collect keys
    for (rj::Value::ConstMemberIterator itr = jsonObject.MemberBegin(); itr != jsonObject.MemberEnd(); ++itr)
    {
        keys.push_back(itr->name.GetString());
    }

    // Sort keys alphabetically
    std::sort(keys.begin(), keys.end());

    // Create a new sorted object
    rj::Value sortedObject(rj::kObjectType);

    // Add members to the sorted object in sorted order
    for (const auto &key : keys)
    {
        rj::Value  name(key.c_str(), allocator);           // Create key as a RapidJSON value
        rj::Value &value = jsonObject[key.c_str()];        // Get corresponding value
        sortedObject.AddMember(name, value, allocator);    // Add key-value pair to sorted object
    }

    // Replace the original object with the sorted one
    jsonObject = std::move(sortedObject);
}

void JsonHandler::SortJsonRecursively(rj::Value &jsonValue, rj::Document::AllocatorType &allocator) const
{
    if (jsonValue.IsObject())
    {
        SortJsonObject(jsonValue, allocator);

        // Recursively sort any nested objects
        for (rj::Value::MemberIterator itr = jsonValue.MemberBegin(); itr != jsonValue.MemberEnd(); ++itr)
        {
            SortJsonRecursively(itr->value, allocator);
        }
    }
    else if (jsonValue.IsArray())
    {
        // If it's an array, sort each element (in case of nested objects)
        for (rj::SizeType i = 0; i < jsonValue.Size(); i++)
        {
            SortJsonRecursively(jsonValue[i], allocator);
        }
    }
}

auto JsonHandler::SortJsonText(const std::string &jsonString) const -> std::string
{
    rj::Document document;

    // TODO: Find some better way
    constexpr auto flgBase_comment = flgBaseReader | rj::kParseCommentsFlag;
    constexpr auto flgBase_comma   = flgBaseReader | rj::kParseTrailingCommasFlag;
    constexpr auto flgBase_Both    = flgBase_comma | flgBase_comment;

    if (m_parseOptions.bIgnoreComment && m_parseOptions.bIgnoreTrailingComma)
    {
        if (document.Parse<flgBase_Both>(jsonString.c_str()).HasParseError())
        {
            return "";
        }
    }

    else if (!m_parseOptions.bIgnoreComment && m_parseOptions.bIgnoreTrailingComma)
    {
        if (document.Parse<flgBase_comma>(jsonString.c_str()).HasParseError())
        {
            return "";
        }
    }

    else if (m_parseOptions.bIgnoreComment && !m_parseOptions.bIgnoreTrailingComma)
    {
        if (document.Parse<flgBase_comment>(jsonString.c_str()).HasParseError())
        {
            return "";
        }
    }

    else if (!m_parseOptions.bIgnoreComment && !m_parseOptions.bIgnoreTrailingComma)
    {
        if (document.Parse<flgBaseReader>(jsonString.c_str()).HasParseError())
        {
            return "";
        }
    }

    SortJsonRecursively(document, document.GetAllocator());

    rj::StringBuffer             buffer;
    rj::Writer<rj::StringBuffer> writer(buffer);
    document.Accept(writer);

    return buffer.GetString();
}
