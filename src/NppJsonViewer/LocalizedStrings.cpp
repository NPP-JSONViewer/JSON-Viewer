#include "Define.h"
#include "resource.h"
#include <tchar.h>
#include <string.h>

// Storage for all localized strings loaded from STRINGTABLE resource.
// Sized generously to hold the longest expected string.
struct LocalizedStrings
{
    TCHAR szPluginName[64];
    TCHAR szTitleJsonPanel[64];
    TCHAR szMenuShowJsonPanel[64];
    TCHAR szMenuFormatJson[64];
    TCHAR szMenuCompressJson[64];
    TCHAR szMenuSortByKey[64];
    TCHAR szMenuSeparator[64];
    TCHAR szMenuSetting[64];
    TCHAR szMenuAbout[64];
    TCHAR szTooltipRefresh[64];
    TCHAR szTooltipValidate[128];
    TCHAR szTooltipFormat[128];
    TCHAR szTooltipSearch[64];
    TCHAR szJsonRoot[32];
    TCHAR szJsonErrorTitle[64];
    TCHAR szJsonWarningTitle[64];
    TCHAR szJsonInfoTitle[64];
    TCHAR szJsonErrParse[256];
    TCHAR szJsonErrValidate[256];
    TCHAR szJsonErrValidateSuccess[256];
    TCHAR szJsonErrSaveSetting[128];
    TCHAR szJsonErrMultiSelection[256];
    TCHAR szStrVersion[32];
    TCHAR szStrCopy[32];
    TCHAR szStrCopyName[32];
    TCHAR szStrCopyValue[32];
    TCHAR szStrCopyPath[32];
    TCHAR szStrExpandAll[32];
    TCHAR szStrCollapseAll[32];
    TCHAR szSrchSearching[64];
    TCHAR szSrchNotFound[64];
    TCHAR szSrchNoMoreFound[64];
};

static LocalizedStrings s_strings = {};
static bool s_bLoaded = false;

// Fallback (English) values - used if LoadString fails for any reason.
#define FALLBACK(name, val) static const TCHAR s_fallback_##name[] = TEXT(val)

FALLBACK(PluginName,             "JSON Viewer");
FALLBACK(TitleJsonPanel,        "JSON Viewer");
FALLBACK(MenuShowJsonPanel,    "Show &JSON Viewer");
FALLBACK(MenuFormatJson,        "&Format JSON");
FALLBACK(MenuCompressJson,      "&Compress JSON");
FALLBACK(MenuSortByKey,        "Sort by &key (ascending)");
FALLBACK(Separator,             "-SEPARATOR-");
FALLBACK(Setting,               "&Settings");
FALLBACK(About,                 "&About");
FALLBACK(TooltipRefresh,        "Refresh JSON tree");
FALLBACK(TooltipValidate,       "Validate JSON to detect any errors");
FALLBACK(TooltipFormat,         "Format JSON to beautify it");
FALLBACK(TooltipSearch,         "Search in JSON");
FALLBACK(JsonRoot,              "JSON");
FALLBACK(ErrorTitle,            "JSON Viewer: Error");
FALLBACK(WarningTitle,          "JSON Viewer: Warning");
FALLBACK(InfoTitle,             "JSON Viewer: Information");
FALLBACK(ErrParse,              "Unable to parse JSON. Please ensure a valid JSON string is selected.");
FALLBACK(ErrValidate,           "An error occurred while parsing the JSON. Check the current selection for the potential issue.");
FALLBACK(ErrValidateSuccess,    "The JSON appears valid. No errors were found during validation.");
FALLBACK(ErrSaveSetting,        "Could not save the settings. Please try again.");
FALLBACK(ErrMultiSelection,     "JSON-Viewer does not currently support multiple selections.");
FALLBACK(StrVersion,            "Version: ");
FALLBACK(StrCopy,               "Copy");
FALLBACK(StrCopyName,           "Copy name");
FALLBACK(StrCopyValue,          "Copy value");
FALLBACK(StrCopyPath,           "Copy path");
FALLBACK(StrExpandAll,          "Expand all");
FALLBACK(StrCollapseAll,        "Collapse all");
FALLBACK(SrchSearching,         "Searching for: ");
FALLBACK(SrchNotFound,          "Not found: ");
FALLBACK(SrchNoMoreFound,       "No more found: ");

#undef FALLBACK

static int LoadStrSafe(HMODULE hMod, UINT uId, LPTSTR pszBuf, int cchBuf, const TCHAR* pszFallback)
{
    int cch = LoadString(hMod, uId, pszBuf, cchBuf);
    if (cch == 0)
    {
        // Fallback to default (English) string
        lstrcpyn(pszBuf, pszFallback, cchBuf);
        cch = static_cast<int>(_tcslen(pszBuf));
    }
    return cch;
}

void LoadLocalizedStrings(HMODULE hModule)
{
    if (s_bLoaded)
        return;
    s_bLoaded = true;

    #define LOAD(field, id, fallback) \
        LoadStrSafe(hModule, id, s_strings.sz##field, _countof(s_strings.sz##field), s_fallback_##fallback)

    LOAD(PluginName,            IDS_PLUGIN_NAME,              PluginName);
    LOAD(TitleJsonPanel,       IDS_TITLE_JSON_PANEL,         TitleJsonPanel);
    LOAD(MenuShowJsonPanel,   IDS_MENU_SHOW_JSON_PANEL,     MenuShowJsonPanel);
    LOAD(MenuFormatJson,       IDS_MENU_FORMAT_JSON,         MenuFormatJson);
    LOAD(MenuCompressJson,     IDS_MENU_COMPRESS_JSON,       MenuCompressJson);
    LOAD(MenuSortByKey,       IDS_MENU_SORT_BY_KEY,         MenuSortByKey);
    LOAD(MenuSeparator,        IDS_MENU_SEPARATOR,           Separator);
    LOAD(MenuSetting,          IDS_MENU_SETTING,             Setting);
    LOAD(MenuAbout,            IDS_MENU_ABOUT,               About);
    LOAD(TooltipRefresh,       IDS_TOOLTIP_REFRESH,          TooltipRefresh);
    LOAD(TooltipValidate,      IDS_TOOLTIP_VALIDATE,         TooltipValidate);
    LOAD(TooltipFormat,        IDS_TOOLTIP_FORMAT,           TooltipFormat);
    LOAD(TooltipSearch,        IDS_TOOLTIP_SEARCH,           TooltipSearch);
    LOAD(JsonRoot,             IDS_JSON_ROOT,                JsonRoot);
    LOAD(JsonErrorTitle,       IDS_JSON_ERROR_TITLE,         ErrorTitle);
    LOAD(JsonWarningTitle,     IDS_JSON_WARNING_TITLE,       WarningTitle);
    LOAD(JsonInfoTitle,        IDS_JSON_INFO_TITLE,          InfoTitle);
    LOAD(JsonErrParse,         IDS_JSON_ERR_PARSE,           ErrParse);
    LOAD(JsonErrValidate,      IDS_JSON_ERR_VALIDATE,        ErrValidate);
    LOAD(JsonErrValidateSuccess, IDS_JSON_ERR_VALIDATE_SUCCESS, ErrValidateSuccess);
    LOAD(JsonErrSaveSetting,   IDS_JSON_ERR_SAVE_SETTING,    ErrSaveSetting);
    LOAD(JsonErrMultiSelection, IDS_JSON_ERR_MULTI_SELECTION, ErrMultiSelection);
    LOAD(StrVersion,           IDS_STR_VERSION,              StrVersion);
    LOAD(StrCopy,              IDS_STR_COPY,                 StrCopy);
    LOAD(StrCopyName,          IDS_STR_COPYNAME,             StrCopyName);
    LOAD(StrCopyValue,         IDS_STR_COPYVALUE,            StrCopyValue);
    LOAD(StrCopyPath,          IDS_STR_COPYPATH,             StrCopyPath);
    LOAD(StrExpandAll,         IDS_STR_EXPANDALL,            StrExpandAll);
    LOAD(StrCollapseAll,       IDS_STR_COLLAPSEALL,          StrCollapseAll);
    LOAD(SrchSearching,        IDS_SRCH_SEARCHING,           SrchSearching);
    LOAD(SrchNotFound,         IDS_SRCH_NOTFOUND,            SrchNotFound);
    LOAD(SrchNoMoreFound,      IDS_SRCH_NOMOREFOUND,         SrchNoMoreFound);

    #undef LOAD
}

// Extern variable definitions - point to the loaded buffer
#define DEFINE_STR(name, field) const TCHAR* name = s_strings.sz##field

DEFINE_STR(PLUGIN_NAME,              PluginName);
DEFINE_STR(TITLE_JSON_PANEL,         TitleJsonPanel);
DEFINE_STR(MENU_SHOW_JSON_PANEL,     MenuShowJsonPanel);
DEFINE_STR(MENU_FORMAT_JSON,         MenuFormatJson);
DEFINE_STR(MENU_COMPRESS_JSON,       MenuCompressJson);
DEFINE_STR(MENU_SORT_BY_KEY,         MenuSortByKey);
DEFINE_STR(MENU_SETTING,             MenuSetting);
DEFINE_STR(MENU_ABOUT,               MenuAbout);
DEFINE_STR(MENU_SEPERATOR,           MenuSeparator);
DEFINE_STR(TOOLTIP_REFRESH,          TooltipRefresh);
DEFINE_STR(TOOLTIP_VALIDATE,         TooltipValidate);
DEFINE_STR(TOOLTIP_FORMAT,           TooltipFormat);
DEFINE_STR(TOOLTIP_SEARCH,           TooltipSearch);
DEFINE_STR(JSON_ROOT,                JsonRoot);
DEFINE_STR(JSON_ERROR_TITLE,         JsonErrorTitle);
DEFINE_STR(JSON_WARNING_TITLE,       JsonWarningTitle);
DEFINE_STR(JSON_INFO_TITLE,          JsonInfoTitle);
DEFINE_STR(JSON_ERR_PARSE,           JsonErrParse);
DEFINE_STR(JSON_ERR_VALIDATE,        JsonErrValidate);
DEFINE_STR(JSON_ERR_VALIDATE_SUCCESS, JsonErrValidateSuccess);
DEFINE_STR(JSON_ERR_SAVE_SETTING,    JsonErrSaveSetting);
DEFINE_STR(JSON_ERR_MULTI_SELECTION, JsonErrMultiSelection);
DEFINE_STR(STR_VERSION,              StrVersion);
DEFINE_STR(STR_COPY,                 StrCopy);
DEFINE_STR(STR_COPYNAME,             StrCopyName);
DEFINE_STR(STR_COPYVALUE,            StrCopyValue);
DEFINE_STR(STR_COPYPATH,             StrCopyPath);
DEFINE_STR(STR_EXPANDALL,            StrExpandAll);
DEFINE_STR(STR_COLLAPSEALL,          StrCollapseAll);
DEFINE_STR(STR_SRCH_SEARCHING,       SrchSearching);
DEFINE_STR(STR_SRCH_NOTFOUND,        SrchNotFound);
DEFINE_STR(STR_SRCH_NOMOREFOUND,     SrchNoMoreFound);

#undef DEFINE_STR
