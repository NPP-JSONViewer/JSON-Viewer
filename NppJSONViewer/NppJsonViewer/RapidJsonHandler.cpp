#include "RapidJsonHandler.h"
#include "JsonViewDlg.h"

const char* const STR_NULL = "null";
const char* const STR_TRUE = "true";
const char* const STR_FALSE = "false";


bool RapidJsonHandler::Null()
{
	if (!m_NodeStack.size())
		return false;

	TreeNode* parent = m_NodeStack.top();
	parent->node.type = JsonNodeType::BOOL;
	parent->node.key = m_strLastKey;
	parent->node.value = STR_NULL;
	m_strLastKey.clear();

	// Print and pop the value
	m_dlg->InsertToTree(parent->subRoot, parent->node.key + " : " + parent->node.value);
	return true;
}

bool RapidJsonHandler::Bool(bool b)
{
	if (!m_NodeStack.size())
		return false;

	TreeNode* parent = m_NodeStack.top();
	parent->node.type = JsonNodeType::BOOL;
	parent->node.key = m_strLastKey;
	parent->node.value = b ? STR_TRUE : STR_FALSE;
	m_strLastKey.clear();

	// Print and pop the value
	m_dlg->InsertToTree(parent->subRoot, parent->node.key + " : " + parent->node.value);
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
	parent->node.type = JsonNodeType::NUMBER;
	parent->node.key = m_strLastKey;
	parent->node.value = str;
	m_strLastKey.clear();

	// Print and pop the value
	m_dlg->InsertToTree(parent->subRoot, parent->node.key + " : " + parent->node.value);
	return true;
}

bool RapidJsonHandler::String(const Ch* str, unsigned /*length*/, bool /*copy*/)
{
	if (!str)
		return false;

	// handle case, when there is only a value in input
	if (m_NodeStack.empty())
	{
		m_dlg->InsertToTree(m_treeRoot, str);
		return true;
	}

	TreeNode* parent = m_NodeStack.top();

	if (parent->node.type != JsonNodeType::ARRAY)
	{
		parent->node.key = m_strLastKey;
		parent->node.value = str;
		m_strLastKey.clear();
	}
	else
	{
		parent->counter++;
	}

	// insert
	m_dlg->InsertToTree(parent->subRoot, parent->node.key + " : \"" + parent->node.value + "\"");

	return true;
}

bool RapidJsonHandler::Key(const Ch* str, unsigned /*length*/, bool /*copy*/)
{
	m_strLastKey = "\"";
	m_strLastKey += str;
	m_strLastKey += "\"";
	return true;
}

bool RapidJsonHandler::StartObject()
{
	TreeNode* parent = nullptr;
	if (m_NodeStack.empty())
	{
		parent = new TreeNode;
		parent->node.type = JsonNodeType::OBJECT;
		parent->subRoot = m_treeRoot;
		parent->counter = 0;
		m_NodeStack.push(parent);
	}
	else
	{
		parent = m_NodeStack.top();
	}

	if (!m_strLastKey.empty() || parent->node.type == JsonNodeType::ARRAY)
	{
		HTREEITEM newNode = nullptr;
		if (parent->node.type != JsonNodeType::ARRAY)
		{
			newNode = m_dlg->InsertToTree(parent->subRoot, m_strLastKey);
			m_strLastKey.clear();
		}
		else
		{
			// It is an array
			std::string arr = "[" + std::to_string(parent->counter) + "]";
			newNode = m_dlg->InsertToTree(parent->subRoot, arr);
		}

		parent->counter++;
		TreeNode* newTreeNode = new TreeNode;
		newTreeNode->node.type = JsonNodeType::OBJECT;
		newTreeNode->subRoot = newNode;
		newTreeNode->counter = 0;
		m_NodeStack.push(newTreeNode);
	}

	return true;
}

bool RapidJsonHandler::EndObject(unsigned /*memberCount*/)
{
	if (!m_NodeStack.empty())
	{
		TreeNode* node = m_NodeStack.top();
		m_NodeStack.pop();
		delete node;
	}
	return true;
}

bool RapidJsonHandler::StartArray()
{
	TreeNode* parent = nullptr;
	if (m_NodeStack.empty())
	{
		parent = new TreeNode;
		parent->node.type = JsonNodeType::ARRAY;
		parent->subRoot = m_treeRoot;
		parent->counter = 0;
		m_NodeStack.push(parent);
		return true;
	}
	else
	{
		parent = m_NodeStack.top();
	}

	if (!m_strLastKey.empty() || parent->node.type == JsonNodeType::ARRAY)
	{
		HTREEITEM newNode;
		if (parent->node.type != JsonNodeType::ARRAY)
		{
			newNode = m_dlg->InsertToTree(parent->subRoot, m_strLastKey);
			m_strLastKey.clear();
		}
		else
		{
			// It is an array
			std::string arr = "[" + std::to_string(parent->counter) + "]";
			newNode = m_dlg->InsertToTree(parent->subRoot, arr);
		}

		parent->counter++;
		TreeNode* newTreeNode = new TreeNode;
		newTreeNode->node.type = JsonNodeType::ARRAY;
		newTreeNode->subRoot = newNode;
		newTreeNode->counter = 0;
		m_NodeStack.push(newTreeNode);
	}
	return true;
}

bool RapidJsonHandler::EndArray(unsigned /*elementCount*/)
{
	if (!m_NodeStack.empty())
	{
		TreeNode* node = m_NodeStack.top();
		m_NodeStack.pop();
		delete node;
	}
	return true;
}

