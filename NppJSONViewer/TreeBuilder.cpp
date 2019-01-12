#include "TreeBuilder.h"
#include <iostream>
#include "JSONDialog.h"
#include "utils.h"
using namespace std;

/*
	1. on Start Object
		push current root to stack
		get current parent
		if(lastKey is not null or parent is array)
			if(parent is not an array)
				get last key
				curRoot = insert into tree (lastKey)
				clear key
			else
				curRoot = insert into tree (counter)
				increment counter
	2. on Start Array
		push current root to stack
		if(currentRoot is not treeRoot)
			get last key
			curRoot = insert into tree (lastKey)
			clear key
	3. on Key
		lastKey = key name
	4. on Value
		get current parent
		if(parent is not an array)
			insert to tree with lastKey:value
			clear key
		else
			insert to tree with counter:value
			increment counter
	5. on End Object
		pop from stack
	6. on End Array
		pop from stack
*/
const char* NULL_STR = "null";
const char* TRUE_STR = "true";
const char* FALSE_STR = "false";

bool TreeBuilder::Null()
{
	return this->String(NULL_STR, static_cast<SizeType>(strlen(NULL_STR)), true);
}

bool TreeBuilder::Bool(bool b)
{
	if (b)
		return this->String(TRUE_STR, static_cast<SizeType>(strlen(TRUE_STR)), true);
	return this->String(FALSE_STR, static_cast<SizeType>(strlen(FALSE_STR)), true);
}

bool TreeBuilder::Int(int i) { cout << "Int(" << i << ")" << endl; return true; }
bool TreeBuilder::Uint(unsigned u) { cout << "Uint(" << u << ")" << endl; return true; }
bool TreeBuilder::Int64(int64_t i) { cout << "Int64(" << i << ")" << endl; return true; }
bool TreeBuilder::Uint64(uint64_t u) { cout << "Uint64(" << u << ")" << endl; return true; }
bool TreeBuilder::Double(double d) { cout << "Double(" << d << ")" << endl; return true; }

bool TreeBuilder::RawNumber(const char *str, SizeType length, bool copy)
{
	return this->String(str, length, copy);
}

bool TreeBuilder::String(const char* str, SizeType length, bool /*copy*/)
{
	// copy and process
	TreeNode *parent = this->stack.top();
	char *value = NULL;
	size_t len = 0;
	if (!parent->isArray)
	{
		len = strlen(this->lastKey) + 3 + length + 1;
		value = new char[len];
		snprintf(value, len, "%s : %s", this->lastKey, str);
		value[len - 1] = '\0';
		delete[] this->lastKey;
		this->lastKey = NULL;
	}
	else
	{
		string strCount = SSTR(parent->counter);
		len = strCount.size() + 3 + length + 1;
		value = new char[len];
		snprintf(value, len, "%s : %s", strCount.c_str(), str);
		value[len - 1] = '\0';
		parent->counter++;
	}

	// insert
	this->dlg->insertToTree(parent->subRoot, value);

	//clear
	delete[] value;
	return true;
}

bool TreeBuilder::StartObject()
{
	TreeNode *parent;
	if (this->stack.empty())
	{
		parent = new TreeNode;
		parent->isArray = false;
		parent->subRoot = treeRoot;
		parent->counter = 0;
		this->stack.push(parent);
	}
	else
	{
		parent = this->stack.top();
	}

	if (this->lastKey != NULL || parent->isArray)
	{
		HTREEITEM newNode;
		if (!parent->isArray)
		{
			newNode = this->dlg->insertToTree(parent->subRoot, this->lastKey);
			delete this->lastKey;
			this->lastKey = NULL;
		}
		else
		{
			newNode = this->dlg->insertToTree(parent->subRoot, SSTR(parent->counter).c_str());
		}

		parent->counter++;
		TreeNode *newTreeNode = new TreeNode;
		newTreeNode->isArray = false;
		newTreeNode->subRoot = newNode;
		newTreeNode->counter = 0;
		this->stack.push(newTreeNode);
	}
	return true;
}

bool TreeBuilder::EndObject(SizeType /*memberCount*/)
{
	if (!this->stack.empty())
	{
		TreeNode *node = this->stack.top();
		this->stack.pop();
		delete node;
	}
	return true;
}

bool TreeBuilder::Key(const char* str, SizeType length, bool /*copy*/)
{
	this->lastKey = new char[length + 1];
	strncpy(this->lastKey, str, length);
	this->lastKey[length] = '\0';
	return true;
}

bool TreeBuilder::StartArray()
{
	TreeNode *parent;
	if (this->stack.empty())
	{
		parent = new TreeNode;
		parent->isArray = false;
		parent->subRoot = treeRoot;
		parent->counter = 0;
		this->stack.push(parent);
	}
	else
	{
		parent = this->stack.top();
	}

	if (this->lastKey != NULL || parent->isArray)
	{
		HTREEITEM newNode;
		if (!parent->isArray)
		{
			newNode = this->dlg->insertToTree(parent->subRoot, this->lastKey);
			delete this->lastKey;
			this->lastKey = NULL;
		}
		else
		{
			newNode = this->dlg->insertToTree(parent->subRoot, SSTR(parent->counter).c_str());
		}
		parent->counter++;
		TreeNode *newTreeNode = new TreeNode;
		newTreeNode->isArray = true;
		newTreeNode->subRoot = newNode;
		newTreeNode->counter = 0;
		this->stack.push(newTreeNode);
	}
	return true;
}

bool TreeBuilder::EndArray(SizeType /*elementCount*/)
{
	if (!this->stack.empty())
	{
		TreeNode *node = this->stack.top();
		this->stack.pop();
		delete node;
	}
	return true;
}

