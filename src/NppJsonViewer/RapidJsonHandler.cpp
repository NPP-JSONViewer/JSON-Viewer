#include "RapidJsonHandler.h"
#include "TreeHandler.h"

const char* const STR_NULL  = "null";
const char* const STR_TRUE  = "true";
const char* const STR_FALSE = "false";


bool RapidJsonHandler::Null()
{
    if (!m_NodeStack.size())
        return false;

    TreeNode* parent = m_NodeStack.top();
    InsertToTree(parent, STR_NULL, false);
    return true;
}

bool RapidJsonHandler::Bool(bool b)
{
    if (!m_NodeStack.size())
        return false;

    TreeNode* parent = m_NodeStack.top();
    InsertToTree(parent, b ? STR_TRUE : STR_FALSE, false);
    return true;
}

bool RapidJsonHandler::Int(int /*i*/)
{
    return true;
}

bool RapidJsonHandler::Uint(unsigned /*i*/)
{
    return true;
}

bool RapidJsonHandler::Int64(int64_t /*i*/)
{
    return true;
}

bool RapidJsonHandler::Uint64(uint64_t /*i*/)
{
    return true;
}

bool RapidJsonHandler::Double(double /*d*/)
{
    return true;
}

bool RapidJsonHandler::RawNumber(const Ch* str, unsigned /*length*/, bool /*copy*/)
{
    if (!m_NodeStack.size())
        return false;

    TreeNode* parent = m_NodeStack.top();
    InsertToTree(parent, str, false);
    return true;
}

bool RapidJsonHandler::String(const Ch* str, unsigned /*length*/, bool /*copy*/)
{
    if (!str)
        return false;

    // handle case, when there is only a value in input
    if (m_NodeStack.empty())
    {
        m_pTreeHandler->InsertToTree(m_treeRoot, str);
        return true;
    }

    TreeNode* parent = m_NodeStack.top();
    InsertToTree(parent, str, true);

    return true;
}

bool RapidJsonHandler::Key(const Ch* str, unsigned length, bool /*copy*/)
{
    m_jsonLastKey.strKey         = str;
    m_jsonLastKey.pos.nLine      = m_pTS->getLine();
    m_jsonLastKey.pos.nColumn    = m_pTS->getColumn() - length - 1;
    m_jsonLastKey.pos.nKeyLength = length;
    return true;
}

bool RapidJsonHandler::StartObject()
{
    TreeNode* parent = nullptr;
    if (m_NodeStack.empty())
    {
        parent            = new TreeNode;
        parent->node.type = JsonNodeType::OBJECT;
        parent->subRoot   = m_treeRoot;
        parent->counter   = 0;
        m_NodeStack.push(parent);
    }
    else
    {
        parent = m_NodeStack.top();
    }

    if (!m_jsonLastKey.strKey.empty() || parent->node.type == JsonNodeType::ARRAY)
    {
        HTREEITEM newNode = nullptr;
        if (parent->node.type != JsonNodeType::ARRAY)
        {
            newNode = m_pTreeHandler->InsertToTree(parent->subRoot, m_jsonLastKey.strKey, m_jsonLastKey.pos);
            m_jsonLastKey.clear();
        }
        else
        {
            // It is an array
            std::string arr = "[" + std::to_string(parent->counter) + "]";
            newNode         = m_pTreeHandler->InsertToTree(parent->subRoot, arr);
        }

        parent->counter++;
        TreeNode* newTreeNode  = new TreeNode;
        newTreeNode->node.type = JsonNodeType::OBJECT;
        newTreeNode->subRoot   = newNode;
        newTreeNode->counter   = 0;
        m_NodeStack.push(newTreeNode);
    }

    return true;
}

bool RapidJsonHandler::EndObject(unsigned memberCount)
{
    AppendNodeCount(memberCount, false);
    return true;
}

bool RapidJsonHandler::StartArray()
{
    TreeNode* parent = nullptr;
    if (m_NodeStack.empty())
    {
        parent            = new TreeNode;
        parent->node.type = JsonNodeType::ARRAY;
        parent->subRoot   = m_treeRoot;
        parent->counter   = 0;
        m_NodeStack.push(parent);
        return true;
    }
    else
    {
        parent = m_NodeStack.top();
    }

    if (!m_jsonLastKey.strKey.empty() || parent->node.type == JsonNodeType::ARRAY)
    {
        HTREEITEM newNode;
        if (parent->node.type != JsonNodeType::ARRAY)
        {
            newNode = m_pTreeHandler->InsertToTree(parent->subRoot, m_jsonLastKey.strKey, m_jsonLastKey.pos);
            m_jsonLastKey.clear();
        }
        else
        {
            // It is an array
            std::string arr = "[" + std::to_string(parent->counter) + "]";
            newNode         = m_pTreeHandler->InsertToTree(parent->subRoot, arr);
        }

        parent->counter++;
        TreeNode* newTreeNode  = new TreeNode;
        newTreeNode->node.type = JsonNodeType::ARRAY;
        newTreeNode->subRoot   = newNode;
        newTreeNode->counter   = 0;
        m_NodeStack.push(newTreeNode);
    }
    return true;
}

bool RapidJsonHandler::EndArray(unsigned elementCount)
{
    AppendNodeCount(elementCount, true);
    return true;
}

void RapidJsonHandler::InsertToTree(TreeNode* node, const char* const str, bool bQuote)
{
    if (!node || !str)
        return;

    if (node->node.type != JsonNodeType::ARRAY)
    {
        node->node.key   = m_jsonLastKey;
        node->node.value = str;
        m_jsonLastKey.clear();
    }
    else
    {
        node->node.key.strKey = "[" + std::to_string(node->counter) + "]";
        node->node.value      = str;

        size_t valueLen               = node->node.value.size();
        node->node.key.pos.nLine      = m_pTS->getLine();
        node->node.key.pos.nColumn    = m_pTS->getColumn() - valueLen - (bQuote ? 1 : 0);    // -1 to deal with double quote in string
        node->node.key.pos.nKeyLength = valueLen;

        node->counter++;
    }

    // Insert item to tree
    if (bQuote)
        m_pTreeHandler->InsertToTree(node->subRoot, node->node.key.strKey + " : \"" + node->node.value + "\"", node->node.key.pos);
    else
        m_pTreeHandler->InsertToTree(node->subRoot, node->node.key.strKey + " : " + node->node.value, node->node.key.pos);
}

void RapidJsonHandler::AppendNodeCount(unsigned elementCount, bool bArray)
{
    if (!m_NodeStack.empty())
    {
        TreeNode* node = m_NodeStack.top();
        m_NodeStack.pop();

        if (node->subRoot && node->subRoot != m_treeRoot)
            m_pTreeHandler->AppendNodeCount(node->subRoot, elementCount, bArray);

        delete node;
    }
}
