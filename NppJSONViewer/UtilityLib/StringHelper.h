#pragma once
#include <string>
#include <vector>
#include <Windows.h>

class StringHelper
{
public:
    StringHelper()  = default;
    ~StringHelper() = default;

    static std::string  ReplaceAll(const std::string &str, const std::string &search, const std::string &replace);
    static std::wstring ReplaceAll(const std::wstring &wstr, const std::wstring &search, const std::wstring &replace);

    static std::wstring ToWstring(const std::string &str, UINT codePage = CP_THREAD_ACP);
    static std::string  ToString(const std::wstring &wstr, UINT codePage = CP_THREAD_ACP);

    static std::vector<std::string>  Split(const std::string &input, const std::string &delim);
    static std::vector<std::wstring> Split(const std::wstring &input, const std::wstring &delim);

    static bool Contains(const std::string &input, const std::string &search, bool ignorecase = true);
    static bool Contains(const std::wstring &input, const std::wstring &search, bool ignorecase = true);

    static void ToLower(std::string &input);
    static void ToLower(std::wstring &input);
};
