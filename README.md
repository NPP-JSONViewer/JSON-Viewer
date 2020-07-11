# JSONViewer 1.40

This plugin is meant to display a JSON string in a Treeview. It also marks the error position in case of parsing errors. That's it!!! The plugin can be used in Notepad++.

## Instructions:
1. Paste the file "NPPJSONViewer.dll" to Notepad++ plugins\NPPJSONViewer folder
2. Restart Notepad++ and verify that the plugin appears under the Plugins menu
3. Open a document containing a JSON string (or paste in some JSON text)
4. Select JSON fragment and navigate to plugins/JSON Viewer/show JSON Viewer or press "Ctrl+Alt+Shift+J"
5. Voila!! If the JSON is valid, it will be shown in a Treeview


## Latest Updates:


### 1.40
1. issue-55 Format JSON should also set the language to JSON #FeatureRequest
2. issue-56 Format Should Follow Line Break Settings
3. issue-57, issue-60 "Should add a function to remove line breaks and spaces" Thanks @neoarc
4. issue-68 Crashing Notepad++
5. issue-72 tab setting from notepad++ settings are not honored
6. issue-73 Display tree for Array of Object 
7. issue-80 use line ending setting from editor
  
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

