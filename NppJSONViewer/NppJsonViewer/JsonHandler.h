#pragma once

#include <string>
#include "rapidjson/prettywriter.h"

struct Result
{
	bool success = false;
	int error_pos = -1;
	int error_code = -1;
	std::string error_str;
	std::string response;
};

using LE = rapidjson::LineEndingOption;
using LF = rapidjson::PrettyFormatOptions;

class JsonHandler
{
public:
	JsonHandler() = default;
	~JsonHandler() = default;

	auto GetCompressedJson(const std::string& jsonText)->const Result;
	auto FormatJson(const std::string& jsonText, LE le, LF lf, char indentChar, unsigned indentLen)->const Result;
	auto ValidateJson(const std::string& jsonText)->const Result;

private:
	auto ParseJson(const std::string& jsonText)->const Result;
};

