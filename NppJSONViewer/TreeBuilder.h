#pragma once

#include "rapidjson/reader.h"
#include<stack>
#include<Windows.h>
#include <commctrl.h>

class JSONDialog;
using namespace rapidjson;

struct TreeNode {
	HTREEITEM subRoot;
	bool isArray;
	int counter;
};


class TreeBuilder : public BaseReaderHandler<UTF8<>, TreeBuilder>
{
	JSONDialog *dlg;
	std::stack<TreeNode*> stack;
	HTREEITEM treeRoot;
	char *lastKey;
public:
	TreeBuilder(JSONDialog *dlg, HTREEITEM treeRoot);
	~TreeBuilder();

	bool Null();
	bool Bool(bool b);
	bool Int(int i);
	bool Uint(unsigned i);
	bool Int64(int64_t i);
	bool Uint64(uint64_t i);
	bool Double(double d);
	bool RawNumber(const char* str, SizeType length, bool copy);
	bool String(const char* str, SizeType length, bool copy);
	bool StartObject();
	bool Key(const char* str, SizeType length, bool copy);
	bool EndObject(SizeType memberCount);
	bool StartArray();
	bool EndArray(SizeType elementCount);

};

