#pragma once
#include "PluginInterface.h"

// Define the number of plugin commands here
enum class CallBackID : int
{
    SHOW_DOC_PANEL = 0,
    FORMAT,
    COMPRESS,
    SEP_1,
    SETTING,
    ABOUT
};
constexpr const int nTotalCommandCount = static_cast<int>(CallBackID::ABOUT) + 1;

// Define plugin name here
const TCHAR PLUGIN_NAME[]   = TEXT("JSON Viewer");
const TCHAR PLUGIN_CONFIG[] = TEXT("JSONViewer.ini");

// Text which can be considered for localization
const TCHAR TITLE_JSON_PANEL[]     = TEXT("JSON Viewer Panel");
const TCHAR MENU_SHOW_JSON_PANEL[] = TEXT("Show &JSON Viewer");
const TCHAR MENU_FORMAT_JSON[]     = TEXT("&Format JSON");
const TCHAR MENU_COMPRESS_JSON[]   = TEXT("&Compress JSON");
const TCHAR MENU_SETTING[]         = TEXT("&Settings");
const TCHAR MENU_ABOUT[]           = TEXT("&About");
const TCHAR MENU_SEPERATOR[]       = TEXT("-SEPARATOR-");

const TCHAR TOOLTIP_REFRESH[]  = TEXT("Refresh JSON tree");
const TCHAR TOOLTIP_VALIDATE[] = TEXT("Validate JSON to detect any errors");
const TCHAR TOOLTIP_FORMAT[]   = TEXT("Format JSON to beautify it");
const TCHAR TOOLTIP_SEARCH[]   = TEXT("Search in JSON");

const TCHAR URL_SOURCE_CODE[]  = TEXT("https://github.com/kapilratnani/JSON-Viewer");
const TCHAR URL_REPORT_ISSUE[] = TEXT("https://github.com/kapilratnani/JSON-Viewer/issues/new");

const TCHAR JSON_ROOT[] = TEXT("JSON");

const TCHAR JSON_ERROR_TITLE[]   = TEXT("JSON Viewer: Error");
const TCHAR JSON_WARNING_TITLE[] = TEXT("JSON Viewer: Warning");
const TCHAR JSON_INFO_TITLE[]    = TEXT("JSON Viewer: Information");

const TCHAR JSON_ERR_PARSE[]            = TEXT("Cannot parse JSON. Please select a JSON string.");
const TCHAR JSON_ERR_VALIDATE[]         = TEXT("There was an error while parsing JSON. Refer to the current selection for possible problematic area.");
const TCHAR JSON_ERR_VALIDATE_SUCCESS[] = TEXT("JSON looks good. No errors found while validating it.");
const TCHAR JSON_ERR_SAVE_SETTING[]     = TEXT("Failed to save the settings. Please try again.");

const TCHAR STR_VERSION[]     = TEXT("Version: ");
const TCHAR STR_COPY[]        = TEXT("Copy");
const TCHAR STR_COPYNAME[]    = TEXT("Copy name");
const TCHAR STR_COPYVALUE[]   = TEXT("Copy value");
const TCHAR STR_COPYPATH[]    = TEXT("Copy path");
const TCHAR STR_EXPANDALL[]   = TEXT("Expand all");
const TCHAR STR_COLLAPSEALL[] = TEXT("Collapse all");

const TCHAR STR_INI_FORMATTING_SEC[]         = TEXT("Formatting");
const TCHAR STR_INI_FORMATTING_EOL[]         = TEXT("EOL");
const TCHAR STR_INI_FORMATTING_LINE[]        = TEXT("LINE_FORMATTING");
const TCHAR STR_INI_FORMATTING_INDENT[]      = TEXT("INDENTATION");
const TCHAR STR_INI_FORMATTING_INDENTCOUNT[] = TEXT("INDENTATION_COUNT");

const TCHAR STR_INI_OTHER_SEC[]               = TEXT("Others");
const TCHAR STR_INI_OTHER_FOLLOW_TAB[]        = TEXT("FOLLOW_TAB");
const TCHAR STR_INI_OTHER_AUTO_FORMAT[]       = TEXT("AUTO_FORMAT");
const TCHAR STR_INI_OTHER_USE_HIGHLIGHT[]     = TEXT("USE_JSON_HIGHLIGHT");
const TCHAR STR_INI_OTHER_IGNORE_COMMENT[]    = TEXT("IGNORE_COMMENT");
const TCHAR STR_INI_OTHER_IGNORE_COMMA[]      = TEXT("IGNORE_TRAILLING_COMMA");
const TCHAR STR_INI_OTHER_REPLACE_UNDEFINED[] = TEXT("REPLACE_VALUE_UNDEFINED");

const TCHAR STR_SRCH_SEARCHING[]   = TEXT("Searching for: ");
const TCHAR STR_SRCH_NOTFOUND[]    = TEXT("Not found: ");
const TCHAR STR_SRCH_NOMOREFOUND[] = TEXT("No more found: ");

enum class LineEnding
{
    AUTO,
    WINDOWS,
    UNIX,
    MAC
};

enum class LineFormat
{
    DEFAULT,
    SINGLELINE
};

enum class IndentStyle
{
    AUTO,
    TAB,
    SPACE
};

struct Indent
{
    unsigned    len   = 4;
    IndentStyle style = IndentStyle::AUTO;
};

struct ParseOptions
{
    bool bIgnoreComment        = true;
    bool bIgnoreTraillingComma = true;
    bool bReplaceUndefined     = false;
};

struct Setting
{
    LineEnding   lineEnding = LineEnding::AUTO;
    LineFormat   lineFormat = LineFormat::DEFAULT;
    Indent       indent {};
    bool         bFollowCurrentTab = false;
    bool         bAutoFormat       = false;
    bool         bUseJsonHighlight = true;
    ParseOptions parseOptions {};
};
