#pragma once
#include "rapidjson/reader.h"
#include "JsonNode.h"

#include <stack>
#include <Windows.h>
#include <commctrl.h>

class JsonViewDlg;

struct TreeNode2
{
	HTREEITEM subRoot;
	JsonNode node;
	int counter;
};

class RapidJsonHandler :public rapidjson::BaseReaderHandler<rapidjson::UTF8<>, RapidJsonHandler>
{
	std::string m_strLastKey;
	std::stack<TreeNode2*> m_NodeStack;

	JsonViewDlg* m_dlg = nullptr;
	HTREEITEM m_treeRoot = nullptr;

public:
	RapidJsonHandler(JsonViewDlg* dlg, HTREEITEM treeRoot) : m_dlg(dlg), m_treeRoot(treeRoot) {}
	virtual ~RapidJsonHandler() = default;

	bool Null();
	bool Bool(bool b);
	bool Int(int i);
	bool Uint(unsigned i);
	bool Int64(int64_t i);
	bool Uint64(uint64_t i);
	bool Double(double d);
	bool RawNumber(const Ch* str, unsigned length, bool copy);
	bool String(const Ch* str, unsigned length, bool copy);
	bool Key(const Ch* str, unsigned length, bool copy);
	bool StartObject();
	bool EndObject(unsigned memberCount);
	bool StartArray();
	bool EndArray(unsigned elementCount);
};

