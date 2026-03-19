#include <regex>
#include <algorithm>
#include <cctype>

#include "StringHelper.h"

// Private helper function to escape regex special characters
std::string StringHelper::EscapeRegex(const std::string& str)
{
    static constexpr std::string_view regex_chars = R"(\.+*?[]{}()|^$)";
    std::string result;
    result.reserve(str.size() * 2);  // Reserve space to avoid reallocations

    for (char c : str)
    {
        if (regex_chars.find(c) != std::string::npos)
        {
            result += '\\';
        }
        result += c;
    }

    return result;
}

std::wstring StringHelper::EscapeRegex(const std::wstring& wstr)
{
    static constexpr std::wstring_view regex_chars = LR"(\.+*?[]{}()|^$)";
    std::wstring result;
    result.reserve(wstr.size() * 2);

    for (wchar_t c : wstr)
    {
        if (regex_chars.find(c) != std::wstring::npos)
        {
            result += L'\\';
        }
        result += c;
    }

    return result;
}

std::string StringHelper::ReplaceAll(const std::string& str, const std::string& search, const std::string& replace)
{
    if (search.empty())
    {
        return str;
    }

    try
    {
        return std::regex_replace(str, std::regex(search), replace);
    }
    catch (const std::regex_error&)
    {
        // If regex is invalid, fall back to literal replacement
        return ReplaceLiteral(str, search, replace);
    }
}

std::wstring StringHelper::ReplaceAll(const std::wstring& wstr, const std::wstring& search, const std::wstring& replace)
{
    if (search.empty())
    {
        return wstr;
    }

    try
    {
        return std::regex_replace(wstr, std::wregex(search), replace);
    }
    catch (const std::regex_error&)
    {
        // If regex is invalid, fall back to literal replacement
        return ReplaceLiteral(wstr, search, replace);
    }
}

std::string StringHelper::ReplaceLiteral(const std::string& str, const std::string& search, const std::string& replace)
{
    if (search.empty())
    {
        return str;
    }

    std::string result;
    result.reserve(str.size());
    size_t lastPos = 0;
    size_t pos = str.find(search);

    while (pos != std::string::npos)
    {
        result.append(str, lastPos, pos - lastPos);
        result.append(replace);
        lastPos = pos + search.length();
        pos = str.find(search, lastPos);
    }

    result.append(str, lastPos);
    return result;
}

std::wstring StringHelper::ReplaceLiteral(const std::wstring& wstr, const std::wstring& search, const std::wstring& replace)
{
    if (search.empty())
    {
        return wstr;
    }

    std::wstring result;
    result.reserve(wstr.size());
    size_t lastPos = 0;
    size_t pos = wstr.find(search);

    while (pos != std::wstring::npos)
    {
        result.append(wstr, lastPos, pos - lastPos);
        result.append(replace);
        lastPos = pos + search.length();
        pos = wstr.find(search, lastPos);
    }

    result.append(wstr, lastPos);
    return result;
}

std::wstring StringHelper::ToWstring(const std::string& str, UINT codePage)
{
    std::wstring wstr;

    if (!str.empty())
    {
        auto required = ::MultiByteToWideChar(codePage, 0, str.data(), static_cast<int>(str.size()), NULL, 0);
        if (0 != required)
        {
            wstr.resize(required);

            auto converted = ::MultiByteToWideChar(codePage, 0, str.data(), static_cast<int>(str.size()), &wstr[0], static_cast<int>(wstr.capacity()));
            if (0 == converted)
            {
                wstr.clear();
            }
        }
    }

    return wstr;
}

std::string StringHelper::ToString(const std::wstring& wstr, UINT codePage)
{
    std::string str;
    if (!wstr.empty())
    {
        auto required = ::WideCharToMultiByte(codePage, 0, wstr.data(), static_cast<int>(wstr.size()), NULL, 0, NULL, NULL);
        if (0 != required)
        {
            str.resize(required);

            auto converted = ::WideCharToMultiByte(codePage, 0, wstr.data(), static_cast<int>(wstr.size()), &str[0], static_cast<int>(str.capacity()), NULL, NULL);
            if (0 == converted)
            {
                str.clear();
            }
        }
    }

    return str;
}

std::vector<std::string> StringHelper::Split(const std::string& input, const std::string& delim)
{
    std::vector<std::string> tokens;

    if (input.empty() || delim.empty())
    {
        if (!input.empty())
        {
            tokens.push_back(input);
        }
        return tokens;
    }

    // Skip delimiters at beginning.
    auto lastPos = input.find_first_not_of(delim, 0);
    // Find first "non-delimiter".
    auto pos = input.find_first_of(delim, lastPos);

    while (pos != std::string::npos || lastPos != std::string::npos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(input.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = input.find_first_not_of(delim, pos);
        // Find next "non-delimiter"
        pos = input.find_first_of(delim, lastPos);
    }

    return tokens;
}

std::vector<std::wstring> StringHelper::Split(const std::wstring& input, const std::wstring& delim)
{
    std::vector<std::wstring> tokens;

    if (input.empty() || delim.empty())
    {
        if (!input.empty())
        {
            tokens.push_back(input);
        }
        return tokens;
    }

    // Skip delimiters at beginning.
    auto lastPos = input.find_first_not_of(delim, 0);
    // Find first "non-delimiter".
    auto pos = input.find_first_of(delim, lastPos);

    while (pos != std::wstring::npos || lastPos != std::wstring::npos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(input.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = input.find_first_not_of(delim, pos);
        // Find next "non-delimiter"
        pos = input.find_first_of(delim, lastPos);
    }

    return tokens;
}

bool StringHelper::Contains(const std::string& input, const std::string& search, bool ignoreCase)
{
    return Contains(ToWstring(input), ToWstring(search), ignoreCase);
}

bool StringHelper::Contains(const std::wstring& input, const std::wstring& search, bool ignoreCase)
{
    std::wstring lower_input  = input;
    std::wstring lower_search = search;
    if (ignoreCase)
    {
        ToLower(lower_input);
        ToLower(lower_search);
    }

    return lower_input.find(lower_search) != std::wstring::npos;
}

void StringHelper::ToLower(std::string& input)
{
    auto s = ToWstring(input);
    ToLower(s);
    input = ToString(s);
}

void StringHelper::ToLower(std::wstring& input)
{
    std::transform(input.begin(), input.end(), input.begin(), ::towlower);
}
