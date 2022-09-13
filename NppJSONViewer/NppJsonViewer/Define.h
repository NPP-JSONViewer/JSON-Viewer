#pragma once
#include "PluginInterface.h"

// Define the number of plugin commands here
const int nTotalCommandCount = 6;

// Define plugin name here
const TCHAR PLUGIN_NAME[] = TEXT("JSON Viewer");

// Text which can be considered for localization
const TCHAR TITLE_JSON_PANEL[] = TEXT("JSON Viewer Panel");
const TCHAR MENU_SHOW_JSON_PANEL[] = TEXT("Show &JSON Viewer");
const TCHAR MENU_FORMAT_JSON[] = TEXT("&Format JSON");
const TCHAR MENU_COMPRESS_JSON[] = TEXT("&Compress JSON");
const TCHAR MENU_OPTION[] = TEXT("&Option");
const TCHAR MENU_ABOUT[] = TEXT("&About");
const TCHAR MENU_SEPERATOR[] = TEXT("-SEPARATOR-");

const TCHAR TOOLTIP_REFRESH[] = TEXT("-Refresh-");
const TCHAR TOOLTIP_VALIDATE[] = TEXT("-Validate-");
const TCHAR TOOLTIP_FORMAT[] = TEXT("-Format-");
const TCHAR TOOLTIP_SEARCH[] = TEXT("-Search-");

const TCHAR URL_SOURCE_CODE[] = TEXT("https://github.com/kapilratnani/JSON-Viewer");
const TCHAR URL_REPORT_ISSUE[] = TEXT("https://github.com/kapilratnani/JSON-Viewer/issues/new");


const TCHAR JSON_ROOT[] = TEXT("JSON");

const TCHAR JSON_ERROR_TITLE[] = TEXT("JSON Viewer: Error");
const TCHAR JSON_WARNING_TITLE[] = TEXT("JSON Viewer: Warning");
const TCHAR JSON_INFO_TITLE[] = TEXT("JSON Viewer: Information");

const TCHAR JSON_ERR_PARSE[] = TEXT("Cannot parse json. Please select a JSON String.");

const TCHAR STR_VERSION[] = TEXT("Version: ");
const TCHAR STR_COPY[] = TEXT("Copy");
const TCHAR STR_COPYNAME[] = TEXT("Copy name"); 
const TCHAR STR_COPYVALUE[] = TEXT("Copy value");
const TCHAR STR_COPYPATH[] = TEXT("Copy path");
const TCHAR STR_EXPANDALL[] = TEXT("Expand all");
const TCHAR STR_COLLAPSEALL[] = TEXT("Collapse all");

