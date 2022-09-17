#include "RapidJsonHandler.h"
#include "JsonViewDlg.h"

const char* const STR_NULL = "null";
const char* const STR_TRUE = "true";
const char* const STR_FALSE = "false";


bool RapidJsonHandler::Null()
{
	if (!m_NodeStack.size())
		return false;

	TreeNode2* parent = m_NodeStack.top();
	parent->node.type = JsonNodeType::BOOL;
	parent->node.key = m_szLastKey;
	parent->node.value = STR_NULL;
	delete[] m_szLastKey;
	m_szLastKey = nullptr;

	// Print and pop the value
	m_dlg->InsertToTree(parent->subRoot, (parent->node.key + " : " + parent->node.value).c_str());
	return true;
}

bool RapidJsonHandler::Bool(bool b)
{
	if (!m_NodeStack.size())
		return false;

	TreeNode2* parent = m_NodeStack.top();
	parent->node.type = JsonNodeType::BOOL;
	parent->node.key = m_szLastKey;
	parent->node.value = b ? STR_TRUE : STR_FALSE;
	delete[] m_szLastKey;
	m_szLastKey = nullptr;

	// Print and pop the value
	m_dlg->InsertToTree(parent->subRoot, (parent->node.key + " : " + parent->node.value).c_str());
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

	TreeNode2* parent = m_NodeStack.top();
	parent->node.type = JsonNodeType::NUMBER;
	parent->node.key = m_szLastKey;
	parent->node.value = str;
	delete[] m_szLastKey;
	m_szLastKey = nullptr;

	// Print and pop the value
	m_dlg->InsertToTree(parent->subRoot, (parent->node.key + " : " + parent->node.value).c_str());
	return true;
}

bool RapidJsonHandler::String(const Ch* str, unsigned /*length*/, bool /*copy*/)
{
	// handle case, when there is only a value in input
	if (m_NodeStack.empty())
	{
		m_dlg->InsertToTree(m_treeRoot, str);
		return true;
	}

	TreeNode2* parent = m_NodeStack.top();

	if (parent->node.type != JsonNodeType::ARRAY)
	{
		parent->node.key = m_szLastKey;
		parent->node.value = str;
		delete[] m_szLastKey;
		m_szLastKey = nullptr;
	}
	else
	{
		/*std::string strCount = std::to_string(parent->counter);
		len = strCount.size() + 3 + length + 1;
		value = new char[len];
		snprintf(value, len, "%s : %s", strCount.c_str(), str);
		value[len - 1] = '\0';*/
		parent->counter++;
	}

	// insert
	m_dlg->InsertToTree(parent->subRoot, (parent->node.key + " : \"" + parent->node.value + "\"").c_str());

	return true;
}

bool RapidJsonHandler::Key(const Ch* str, unsigned length, bool /*copy*/)
{
	m_szLastKey = new char[length + 1];
	strncpy_s(m_szLastKey, length + 1, str, length);
	m_szLastKey[length] = '\0';
	return true;
}

bool RapidJsonHandler::StartObject()
{
	TreeNode2* parent = nullptr;
	if (m_NodeStack.empty())
	{
		parent = new TreeNode2;
		parent->node.type = JsonNodeType::OBJECT;
		parent->subRoot = m_treeRoot;
		parent->counter = 0;
		m_NodeStack.push(parent);
	}
	else
	{
		parent = m_NodeStack.top();
	}

	if (m_szLastKey != nullptr || parent->node.type == JsonNodeType::ARRAY)
	{
		HTREEITEM newNode = nullptr;
		if (parent->node.type != JsonNodeType::ARRAY)
		{
			newNode = m_dlg->InsertToTree(parent->subRoot, m_szLastKey);
			delete[] m_szLastKey;
			m_szLastKey = nullptr;
		}
		else
		{
			// It is an array
			std::string arr = "[" + std::to_string(parent->counter) + "]";
			newNode = m_dlg->InsertToTree(parent->subRoot, arr.c_str());
		}

		parent->counter++;
		TreeNode2* newTreeNode = new TreeNode2;
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
		TreeNode2* node = m_NodeStack.top();
		m_NodeStack.pop();
		delete node;
	}
	return true;
}

bool RapidJsonHandler::StartArray()
{
	TreeNode2* parent = nullptr;
	if (m_NodeStack.empty())
	{
		parent = new TreeNode2;
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

	if (m_szLastKey != nullptr || parent->node.type == JsonNodeType::ARRAY)
	{
		HTREEITEM newNode;
		if (parent->node.type != JsonNodeType::ARRAY)
		{
			newNode = m_dlg->InsertToTree(parent->subRoot, m_szLastKey);
			delete[] m_szLastKey;
			m_szLastKey = nullptr;
		}
		else
		{
			// It is an array
			std::string arr = "[" + std::to_string(parent->counter) + "]";
			newNode = m_dlg->InsertToTree(parent->subRoot, arr.c_str());
		}

		parent->counter++;
		TreeNode2* newTreeNode = new TreeNode2;
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
		TreeNode2* node = m_NodeStack.top();
		m_NodeStack.pop();
		delete node;
	}
	return true;
}

