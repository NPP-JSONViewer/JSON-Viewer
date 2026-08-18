#pragma once
#include "PluginInterface.h"

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

// Define plugin name here
const TCHAR PLUGIN_NAME[]   = TEXT("JSON Viewer");
const TCHAR PLUGIN_CONFIG[] = TEXT("JSONViewer.ini");

// Text which can be considered for localization
const TCHAR TITLE_JSON_PANEL[]     = TEXT("JSON Viewer");
const TCHAR MENU_SHOW_JSON_PANEL[] = TEXT("显示 JSON 查看器(&J)");
const TCHAR MENU_FORMAT_JSON[]     = TEXT("格式化 JSON(&F)");
const TCHAR MENU_COMPRESS_JSON[]   = TEXT("压缩 JSON(&C)");
const TCHAR MENU_SORT_BY_KEY[]     = TEXT("按键排序（升序）(&K)");
const TCHAR MENU_SETTING[]         = TEXT("设置(&S)");
const TCHAR MENU_ABOUT[]           = TEXT("关于(&A)");
const TCHAR MENU_SEPERATOR[]       = TEXT("-SEPARATOR-");

const TCHAR TOOLTIP_REFRESH[]  = TEXT("刷新 JSON 树");
const TCHAR TOOLTIP_VALIDATE[] = TEXT("验证 JSON 是否存在错误");
const TCHAR TOOLTIP_FORMAT[]   = TEXT("格式化 JSON，美化显示");
const TCHAR TOOLTIP_SEARCH[]   = TEXT("在 JSON 中搜索");

const TCHAR URL_SOURCE_CODE[]  = TEXT("https://github.com/NPP-JSONViewer/JSON-Viewer");
const TCHAR URL_REPORT_ISSUE[] = TEXT("https://github.com/NPP-JSONViewer/JSON-Viewer/issues/new");

const TCHAR JSON_ROOT[] = TEXT("JSON");

const TCHAR JSON_ERROR_TITLE[]   = TEXT("JSON 查看器：错误");
const TCHAR JSON_WARNING_TITLE[] = TEXT("JSON 查看器：警告");
const TCHAR JSON_INFO_TITLE[]    = TEXT("JSON 查看器：提示");

const TCHAR JSON_ERR_PARSE[]            = TEXT("无法解析 JSON。请确保已选择有效的 JSON 字符串。");
const TCHAR JSON_ERR_VALIDATE[]         = TEXT("解析 JSON 时出错。请检查当前选中内容是否存在问题。");
const TCHAR JSON_ERR_VALIDATE_SUCCESS[] = TEXT("JSON 验证通过。验证过程中未发现错误。");
const TCHAR JSON_ERR_SAVE_SETTING[]     = TEXT("无法保存设置，请重试。");
const TCHAR JSON_ERR_MULTI_SELECTION[]  = TEXT("JSON 查看器目前不支持多选。");

const TCHAR STR_VERSION[]     = TEXT("版本：");
const TCHAR STR_COPY[]        = TEXT("复制");
const TCHAR STR_COPYNAME[]    = TEXT("复制名称");
const TCHAR STR_COPYVALUE[]   = TEXT("复制值");
const TCHAR STR_COPYPATH[]    = TEXT("复制路径");
const TCHAR STR_EXPANDALL[]   = TEXT("全部展开");
const TCHAR STR_COLLAPSEALL[] = TEXT("全部折叠");

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

const TCHAR STR_SRCH_SEARCHING[]   = TEXT("正在搜索：");
const TCHAR STR_SRCH_NOTFOUND[]    = TEXT("未找到：");
const TCHAR STR_SRCH_NOMOREFOUND[] = TEXT("无更多结果：");

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
