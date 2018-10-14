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

bool TreeBuilder::Null() {
	return this->String(NULL_STR, sizeof(NULL_STR), true);
}

bool TreeBuilder::Bool(bool b) {
	if (b)
		return this->String(TRUE_STR, sizeof(TRUE_STR), true);
	return this->String(FALSE_STR, sizeof(FALSE_STR), true);
}

bool TreeBuilder::Int(int i) { return true; }
bool TreeBuilder::Uint(unsigned u) {  return true; }
bool TreeBuilder::Int64(int64_t i) {  return true; }
bool TreeBuilder::Uint64(uint64_t u) { return true; }
bool TreeBuilder::Double(double d) {  return true; }

bool TreeBuilder::RawNumber(const char *str, SizeType length, bool copy) {
	return this->String(str, length, copy);
}

bool TreeBuilder::String(const char* str, SizeType length, bool copy) {
	// copy and process
	TreeNode *parent = this->stack.top();
	string path = parent->path;
	char *value = NULL;
	int len;
	if (!parent->isArray) {
		len = strlen(this->lastKey) + 3 + length + 1;
		value = new char[len];
		snprintf(value, len, "%s : %s", this->lastKey, str);
		value[len - 1] = '\0';
		path = path.append(this->lastKey).append("/");
		delete[] this->lastKey;
		this->lastKey = NULL;
	}
	else {
		string strCount = SSTR(parent->counter);
		len = strCount.size() + 3 + length + 1;
		value = new char[len];
		snprintf(value, len, "%s : %s", strCount.c_str(), str);
		value[len - 1] = '\0';
		path = path.append(strCount).append("/");
		parent->counter++;
	}

	// insert
	this->dlg->insertToTree(parent->subRoot, value, path.c_str());

	//clear
	delete[] value;
	return true;
}

bool TreeBuilder::StartObject() {
	TreeNode *parent;
	if (this->stack.empty()) {
		parent = new TreeNode;
		parent->isArray = false;
		parent->subRoot = treeRoot;
		parent->counter = 0;
		parent->path = string("/");
		this->stack.push(parent);
	}
	else {
		parent = this->stack.top();
	}

	if (this->lastKey != NULL || parent->isArray) {
		HTREEITEM newNode;
		string path = parent->path;
		if (!parent->isArray) {
			path = path.append(this->lastKey).append("/");
			newNode = this->dlg->insertToTree(parent->subRoot, this->lastKey, path.c_str());
			delete this->lastKey;
			this->lastKey = NULL;
		}
		else {
			path = path.append(SSTR(parent->counter)).append("/");
			newNode = this->dlg->insertToTree(parent->subRoot, SSTR(parent->counter).c_str(), path.c_str());
		}

		parent->counter++;
		TreeNode *newTreeNode = new TreeNode;
		newTreeNode->isArray = false;
		newTreeNode->subRoot = newNode;
		newTreeNode->counter = 0;
		newTreeNode->path = path;
		this->stack.push(newTreeNode);
	}
	return true;
}

bool TreeBuilder::EndObject(SizeType memberCount) {
	if (!this->stack.empty()) {
		TreeNode *node = this->stack.top();
		this->stack.pop();
		delete node;
	}
	return true;
}

bool TreeBuilder::Key(const char* str, SizeType length, bool copy) {
	this->lastKey = new char[length + 1];
	strncpy(this->lastKey, str, length);
	this->lastKey[length] = '\0';
	return true;
}

bool TreeBuilder::StartArray() {
	TreeNode *parent;
	if (this->stack.empty()) {
		parent = new TreeNode;
		parent->isArray = false;
		parent->subRoot = treeRoot;
		parent->counter = 0;
		parent->path = string("/");
		this->stack.push(parent);
	}
	else {
		parent = this->stack.top();
	}

	if (this->lastKey != NULL || parent->isArray) {
		HTREEITEM newNode;
		string path = parent->path;
		if (!parent->isArray) {
			path = path.append(this->lastKey).append("/");
			newNode = this->dlg->insertToTree(parent->subRoot, this->lastKey, path.c_str());
			delete this->lastKey;
			this->lastKey = NULL;
		}
		else {
			path = path.append(SSTR(parent->counter)).append("/");
			newNode = this->dlg->insertToTree(parent->subRoot, SSTR(parent->counter).c_str(), path.c_str());
		}
		parent->counter++;
		TreeNode *newTreeNode = new TreeNode;
		newTreeNode->isArray = true;
		newTreeNode->subRoot = newNode;
		newTreeNode->counter = 0;
		newTreeNode->path = path;
		this->stack.push(newTreeNode);
	}
	return true;
}

bool TreeBuilder::EndArray(SizeType elementCount) {
	if (!this->stack.empty()) {
		TreeNode *node = this->stack.top();
		this->stack.pop();
		delete node;
	}
	return true;
}

TreeBuilder::TreeBuilder(JSONDialog *dlg, HTREEITEM treeRoot) {
	this->dlg = dlg;
	this->lastKey = NULL;
	this->treeRoot = treeRoot;
}


TreeBuilder::~TreeBuilder()
{
}

