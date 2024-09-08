#include "StringHelper.h"
#include <regex>

std::string StringHelper::ReplaceAll(const std::string &str, const std::string &search, const std::string &replace)
{
    return std::regex_replace(str, std::regex(search), replace);
}

std::wstring StringHelper::ReplaceAll(const std::wstring &wstr, const std::wstring &search, const std::wstring &replace)
{
    return std::regex_replace(wstr, std::wregex(search), replace);
}

std::wstring StringHelper::ToWstring(const std::string &str, UINT codePage)
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

std::string StringHelper::ToString(const std::wstring &wstr, UINT codePage)
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

std::vector<std::string> StringHelper::Split(const std::string &input, const std::string &delim)
{
    // Vector is created on stack and copied on return
    std::vector<std::string> tokens;

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

std::vector<std::wstring> StringHelper::Split(const std::wstring &input, const std::wstring &delim)
{
    // Vector is created on stack and copied on return
    std::vector<std::wstring> tokens;

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

bool StringHelper::Contains(const std::string &input, const std::string &search, bool ignorecase)
{
    return Contains(ToWstring(input), ToWstring(search), ignorecase);
}

bool StringHelper::Contains(const std::wstring &input, const std::wstring &search, bool ignorecase)
{
    std::wstring lower_input  = input;
    std::wstring lower_search = search;
    if (ignorecase)
    {
        ToLower(lower_input);
        ToLower(lower_search);
    }

    return lower_input.find(lower_search) != std::wstring::npos;
}

void StringHelper::ToLower(std::string &input)
{
    auto s = ToWstring(input);
    ToLower(s);
    input = ToString(s);
}

void StringHelper::ToLower(std::wstring &input)
{
    std::transform(input.begin(), input.end(), input.begin(), ::towlower);
}
