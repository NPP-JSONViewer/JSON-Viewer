#include "JsonHandler.h"

#include "rapidjson/reader.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

auto JsonHandler::GetCompressedJson(const std::string& jsonText)-> const Result
{
	return ParseJson(jsonText);
}

auto JsonHandler::FormatJson(const std::string& /*jsonText*/)-> const Result
{
	return Result();
}


auto JsonHandler::ValidateJson(const std::string& jsonText)-> const Result
{
	return ParseJson(jsonText);
}

auto JsonHandler::ParseJson(const std::string& jsonText) -> const Result
{
	Result retVal{};

	rapidjson::StringBuffer sb;
	rapidjson::Writer<rapidjson::StringBuffer> pw(sb);
	rapidjson::StringStream ss(jsonText.c_str());
	rapidjson::Reader reader;

	if (reader.Parse<rapidjson::kParseFullPrecisionFlag | rapidjson::kParseCommentsFlag |
		rapidjson::kParseEscapedApostropheFlag | rapidjson::kParseNanAndInfFlag | rapidjson::kParseTrailingCommasFlag>(ss, pw)
		&& sb.GetString())
	{
		retVal.success = true;
		retVal.response = sb.GetString();
		retVal.error_str.clear();
		retVal.error_code = retVal.error_pos = -1;
	}
	else
	{
		retVal.success = false;
		retVal.response.clear();
		retVal.error_str = rapidjson::GetParseError_En(reader.GetParseErrorCode());
		retVal.error_pos = static_cast<int>(reader.GetErrorOffset());
		retVal.error_code = reader.GetParseErrorCode();
	}

	return retVal;
}

