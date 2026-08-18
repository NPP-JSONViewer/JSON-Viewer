#pragma once
#include "PluginInterface.h"
#include <windows.h>

// Define the number of plugin commands here
enum class CallBackID : int
{
    SHOW_DOC_PANEL = 0,
    FORMAT,
    COMPRESS,
    SORT_BY_KEY,
    SEP_1,
    SETTING,
    ABOUT
};
constexpr const int nTotalCommandCount = static_cast<int>(CallBackID::ABOUT) + 1;

// ---------------------------------------------------------------------------
// Localizable UI strings.
// These are loaded from the STRINGTABLE resource at init time, so they
// automatically follow the system's UI language (English, Chinese, etc.)
// ---------------------------------------------------------------------------
void LoadLocalizedStrings(HMODULE hModule);

// Plugin name and config filename (config filename must not be localized)
extern const TCHAR* PLUGIN_NAME;
const TCHAR PLUGIN_CONFIG[] = TEXT("JSONViewer.ini");

// Menu and panel
extern const TCHAR* TITLE_JSON_PANEL;
extern const TCHAR* MENU_SHOW_JSON_PANEL;
extern const TCHAR* MENU_FORMAT_JSON;
extern const TCHAR* MENU_COMPRESS_JSON;
extern const TCHAR* MENU_SORT_BY_KEY;
extern const TCHAR* MENU_SETTING;
extern const TCHAR* MENU_ABOUT;
extern const TCHAR* MENU_SEPERATOR;

// Tooltips
extern const TCHAR* TOOLTIP_REFRESH;
extern const TCHAR* TOOLTIP_VALIDATE;
extern const TCHAR* TOOLTIP_FORMAT;
extern const TCHAR* TOOLTIP_SEARCH;

// URLs (not localized)
const TCHAR URL_SOURCE_CODE[]  = TEXT("https://github.com/NPP-JSONViewer/JSON-Viewer");
const TCHAR URL_REPORT_ISSUE[] = TEXT("https://github.com/NPP-JSONViewer/JSON-Viewer/issues/new");

// Misc
extern const TCHAR* JSON_ROOT;

// Message titles
extern const TCHAR* JSON_ERROR_TITLE;
extern const TCHAR* JSON_WARNING_TITLE;
extern const TCHAR* JSON_INFO_TITLE;

// Message content
extern const TCHAR* JSON_ERR_PARSE;
extern const TCHAR* JSON_ERR_VALIDATE;
extern const TCHAR* JSON_ERR_VALIDATE_SUCCESS;
extern const TCHAR* JSON_ERR_SAVE_SETTING;
extern const TCHAR* JSON_ERR_MULTI_SELECTION;

// Context menu and misc
extern const TCHAR* STR_VERSION;
extern const TCHAR* STR_COPY;
extern const TCHAR* STR_COPYNAME;
extern const TCHAR* STR_COPYVALUE;
extern const TCHAR* STR_COPYPATH;
extern const TCHAR* STR_EXPANDALL;
extern const TCHAR* STR_COLLAPSEALL;

// INI section/key names (must NOT be localized - config compatibility)
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

// Search status
extern const TCHAR* STR_SRCH_SEARCHING;
extern const TCHAR* STR_SRCH_NOTFOUND;
extern const TCHAR* STR_SRCH_NOMOREFOUND;

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
    bool bIgnoreComment       = true;
    bool bIgnoreTrailingComma = true;
    bool bReplaceUndefined    = false;
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
