#pragma once
#include "PluginInterface.h"

// Define the number of plugin commands here
enum class CallBackID :int { SHOW_DOC_PANEL = 0, FORMAT, COMPRESS, SEP_1, SETTING, ABOUT };
constexpr const int nTotalCommandCount = static_cast<int>(CallBackID::ABOUT) + 1;

// Define plugin name here
const TCHAR PLUGIN_NAME[] = TEXT("JSON Viewer");
const TCHAR PLUGIN_CONFIG[] = TEXT("JSONViewer.ini");

// Text which can be considered for localization
const TCHAR TITLE_JSON_PANEL[] = TEXT("JSON Viewer Panel");
const TCHAR MENU_SHOW_JSON_PANEL[] = TEXT("Show &JSON Viewer");
const TCHAR MENU_FORMAT_JSON[] = TEXT("&Format JSON");
const TCHAR MENU_COMPRESS_JSON[] = TEXT("&Compress JSON");
const TCHAR MENU_SETTING[] = TEXT("&Setting");
const TCHAR MENU_ABOUT[] = TEXT("&About");
const TCHAR MENU_SEPERATOR[] = TEXT("-SEPARATOR-");

const TCHAR TOOLTIP_REFRESH[] = TEXT("Refresh JSON tree");
const TCHAR TOOLTIP_VALIDATE[] = TEXT("Validate JSON to detect any error");
const TCHAR TOOLTIP_FORMAT[] = TEXT("Format JSON to beautify it");
const TCHAR TOOLTIP_SEARCH[] = TEXT("Search in JSON");

const TCHAR URL_SOURCE_CODE[] = TEXT("https://github.com/kapilratnani/JSON-Viewer");
const TCHAR URL_REPORT_ISSUE[] = TEXT("https://github.com/kapilratnani/JSON-Viewer/issues/new");


const TCHAR JSON_ROOT[] = TEXT("JSON");

const TCHAR JSON_ERROR_TITLE[] = TEXT("JSON Viewer: Error");
const TCHAR JSON_WARNING_TITLE[] = TEXT("JSON Viewer: Warning");
const TCHAR JSON_INFO_TITLE[] = TEXT("JSON Viewer: Information");

const TCHAR JSON_ERR_PARSE[] = TEXT("Cannot parse JSON. Please select a JSON String.");
const TCHAR JSON_ERR_VALIDATE[] = TEXT("There was an error while parsing JSON. Refer the current selection for possible problematic area.");
const TCHAR JSON_ERR_VALIDATE_SUCCESS[] = TEXT("JSON looks good. No error found while validating it.");

const TCHAR STR_VERSION[] = TEXT("Version: ");
const TCHAR STR_COPY[] = TEXT("Copy");
const TCHAR STR_COPYNAME[] = TEXT("Copy name");
const TCHAR STR_COPYVALUE[] = TEXT("Copy value");
const TCHAR STR_COPYPATH[] = TEXT("Copy path");
const TCHAR STR_EXPANDALL[] = TEXT("Expand all");
const TCHAR STR_COLLAPSEALL[] = TEXT("Collapse all");

const TCHAR STR_INI_FORMATTING_SEC[] = TEXT("FORMATTING");
const TCHAR STR_INI_FORMATTING_EOL[] = TEXT("EOL");
const TCHAR STR_INI_FORMATTING_LINE[] = TEXT("LINE_FORMATTING");
const TCHAR STR_INI_FORMATTING_INDENT[] = TEXT("INDENTATION");
const TCHAR STR_INI_FORMATTING_INDENTCOUNT[] = TEXT("INDENTATION_COUNT");

enum class IndentStyle { AUTO, TAB, SPACE };
struct Indent
{
	unsigned len = 0;
	IndentStyle style = IndentStyle::AUTO;
};

enum class LineEnding { AUTO, WINDOWS, UNIX, MAC };
enum class LineFormat { DEFAULT, SINGLELINE };

struct Setting
{
	LineEnding le = LineEnding::AUTO;
	LineFormat lf = LineFormat::DEFAULT;
	Indent indent{};
};

