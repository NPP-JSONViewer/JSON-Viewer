#pragma once

#include <stack>
#include <memory>
#include <Windows.h>
#include <commctrl.h>

#include <rapidjson/reader.h>

#include "JsonNode.h"
#include "TrackingStream.h"

class JsonViewDlg;

struct TreeNode
{
    HTREEITEM subRoot {};
    JsonNode  node {};
    int       counter {};
};


class RapidJsonHandler : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>, RapidJsonHandler>
{
    JsonKey               m_jsonLastKey {};
    std::stack<TreeNode*> m_NodeStack;

    TrackingStreamSharedPtr m_pTS;
    JsonViewDlg*            m_dlg      = nullptr;
    HTREEITEM               m_treeRoot = nullptr;

public:
    RapidJsonHandler(JsonViewDlg* dlg, HTREEITEM treeRoot, TrackingStreamSharedPtr pTS = nullptr)
        : m_pTS(pTS ? pTS->GetShared() : nullptr)
        , m_dlg(dlg)
        , m_treeRoot(treeRoot)
    {
    }
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

private:
    void InsertToTree(TreeNode* node, const char* const str, bool bQuote);
    void AppendNodeCount(unsigned elementCount, bool bArray);
};
