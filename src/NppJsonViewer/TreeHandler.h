#pragma once

#include <string>
#include <Windows.h>
#include <CommCtrl.h>

#include "JsonNode.h"

class TreeHandler
{
public:
    virtual ~TreeHandler() = default;

    virtual HTREEITEM InsertToTree(HTREEITEM parent, const std::string& text)                      = 0;
    virtual HTREEITEM InsertToTree(HTREEITEM parent, const std::string& text, const Position& pos) = 0;
    virtual void      AppendNodeCount(HTREEITEM node, unsigned elementCount, bool bArray)          = 0;
};
