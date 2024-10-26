# JSONViewer for Notepad++
[![GitHub release](https://img.shields.io/github/release/kapilratnani/JSON-Viewer.svg)](../../releases/latest)
&nbsp;&nbsp;&nbsp;&nbsp;[![CI Build](https://github.com/kapilratnani/JSON-Viewer/actions/workflows/ci_build.yml/badge.svg)](https://github.com/kapilratnani/JSON-Viewer/actions/workflows/ci_build.yml)
&nbsp;&nbsp;&nbsp;&nbsp;[![Code Scanning](https://github.com/kapilratnani/JSON-Viewer/actions/workflows/codeql.yml/badge.svg)](https://github.com/kapilratnani/JSON-Viewer/actions/workflows/codeql.yml)

This plugin is designed to display JSON strings in a Treeview format and highlight the error position if any parsing issues occur. It's a simple and efficient tool, compatible with [Notepad++](https://github.com/notepad-plus-plus/notepad-plus-plus).


## Instructions:
1. Copy the file `NPPJSONViewer.dll` to the `plugins\NPPJSONViewer` folder in the Notepad++ installation directory..
2. Restart Notepad++ and ensure the plugin appears under the Plugins menu.
3. Open a document containing a JSON string (or paste in some JSON text).
4. Select the JSON fragment and go to <b>Plugins > JSON Viewer > Show JSON Viewer</b> or press  or press <kbd>Ctrl</kbd>+<kbd>Alt</kbd>+<kbd>Shift</kbd>+<kbd>J</kbd>.
5. Voila, that's it! If the JSON is valid, it will be displayed in a Treeview format.


## Latest Updates:

### 2.1.0.0

1. New features:
    1. Navigate directly to the JSON node in the editor upon node selection (using left mouse click or arrow keys).
    2. Select the JSON key in the editor when double-clicking on a node.


2. Bug/regression fixes:
    1. Updated to the latest RapidJSON parser.
    2. Minor code enhancements for improved performance.
    3. Added unit tests


### 2.0.8.0

1. New features:
    1. Sort ascending by key
    2. Add file name in the title for visibility


2. Bug/regression fixes:
    1. Updated license text on UI as per GitHub link
    2. Excplicit callout for no support on multi selection
    3. Some other minor code and UI enhancements


### 2.0.7.0
Bug/regression fixes:
1. "Copy value" / "Copy" not always return complete text
2. Format JSON option does not work if focused tab is in other view
3. Corrected some typos on setting dialog
4. Updated icons to match with dark mode as well
5. Removed quotes from the key in Treeview
6. Some other minor enhancements

### 2.0.6.0
1. New feature:
    1. Replace value 'undefined' with 'null'. This is configurable feature.
2. Bug/regression fixes:
    1. Cyrillic text is not properly shown in json tree view dialog

### 2.0.5.0
1. New feature:
    1. Make json highlighter configurable
2. Bug/regression fixes:
    1. Handle json for both the views.
    2. Update treeview on reopen
    3. When file type is json, then error message is shown twice on npp launch if viewer dock was kept opened on previous instance
    4. Don't show error message on startup for non json files

### 2.0.4.0
1. New feature:
    1. Show element count for list/array
2. Bug/regression fixes:
    1. Crash fix: Setting dialog is not shown if about dialog is opened before it.
    2. Set language type JSON properly
    3. Some minor UI enhancements

### 2.0.3.0
1. New feature:
    1. Search in json tree window
    2. Handle NaN, Inf, -Inf, Infinity, -Infinity properly
2. Bug/regression fixes:
    1. Handle all types of arrays which does not have any key
    2. Corrected typos in setting json
    3. Don't use double qoutes for other than string type


### 2.0.2.0
1. Provided UI to control formatting option via setting dialog
    1. Setting for indentation
    2. Setting for line ending
    3. Setting for line format
    4. Make json parsing configurable e.g. ignore trailing comma, ignore comment
2. Added couple of new feature
    1. Follow json tree for current tab if it is json file
    2. Auto format json file when opened (by direct or by tab switching)
3. Few bug/regression fixes

### 2.0.1.0
1. Redeveloped UI 
    1. Provided menu icon
    2. Json view panel is redesigned which is button like, refresh, validate, format etc.
    3. It uses well performed class instead of plain function
2. Current selected node path is given on the bottom of json view window
3. Many feature support such as copy node, copy value, copy path, expand/collapse all etc.
4. Few bug fixes


### 1.41
1. Support for ARM64
2. Dropped Windows XP support as Notepad++ is no more supporting Windows XP.
3. Upgrade Visual Studio to 2022


### 1.40
1. issue-55 Format JSON should also set the language to JSON #FeatureRequest
2. issue-56 Format Should Follow Line Break Settings
3. issue-57, issue-60 "Should add a function to remove line breaks and spaces" Thanks @neoarc
4. issue-68 Crashing Notepad++
5. issue-72 tab setting from notepad++ settings are not honored
6. issue-73 Display tree for Array of Object 
7. issue-80 use line ending setting from editor
8. Relaxed parsing. Supports trailling commas, comments(only parsing), NaN and infinity JS literals.
  
### 1.34
1. Fix Access Violation issue #51
2. Fix issue #47
3. Fix issue #43

### 1.31
1. Reads tab setting from notepad++ settings

### 1.30a
1. Now using rapidjson

### 1.24
1. 64 bit support. Thanks @chcg
   
### 1.23
1. Select all text when no selection
   Thanks @vakio
2. fix memory leak and close About dialog when clicking "Close" button
   Thanks @quangnh89 

### 1.22
1. Fixed display of boolean values. Now displaying as "key":True/False
   Thanks @yoyokenny
2. Fixed hang on faulty JSON.
   Thanks @vancekic

### 1.21
1. Fixed display of UTF-8 characters.

### 1.20
1. Fixed bug "#3 quoted doublequotes-Jan Huschauer"

### 1.19
1. Added a command to format JSON

### 1.175
1. Now displays a message box when JSON string is not selected.

### 1.17
1. Fixed dialog display issue, that occured in 1.16 release.

### 1.16
1. Fixed bug - 3305433 do not find error on second try and do not build tree
2. Fixed a memory leak..was not deallocating memory allocated to json strings

### 1.15
1. Fixed bug 3203739 "Unable to parse JSON Arrays"

### 1.1
1. Fixed hotkey, now press CTRL+SHIFT+ALT+J(default).
2. Marks error position in JSON


## Contributors

<div align="center">

<a href="https://github.com/kapilratnani/JSON-Viewer/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=kapilratnani/JSON-Viewer" />
</a>
