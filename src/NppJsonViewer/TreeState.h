#pragma once

#include <optional>
#include <string>
#include <vector>

#include "JsonNode.h"

/*
 * TreeState is a per-tab snapshot of the JSON tree: the nodes themselves
 * (text and editor position), their expansion state and the current selection.
 * It is a plain in-memory model, independent from any Win32 control.
 *
 * It is used to remember what a tab looked like: when the user switches away
 * from a tab and later comes back, the tree is restored from the snapshot
 * instead of being parsed again. See TreeExpansion.h for the lighter-weight
 * "same document, tree rebuilt" case (refresh).
 *
 * Only the children of the tree root ("JSON") are captured; the root itself is
 * always recreated by TreeViewCtrl::InitTree().
 */
struct TreeStateNode
{
    std::wstring              text;      // Display text of the node (including trailing [n]/{n} counts)
    std::optional<Position>   pos;       // Editor position of the key (nullopt when the node has none)
    bool                      expanded = false;
    std::vector<TreeStateNode> children;
};

struct TreeState
{
    std::vector<TreeStateNode> roots;          // Children of the tree root ("JSON")
    std::vector<std::wstring>  selectedPath;   // Path of the selected node (key per level), empty when none
};

class TreeStateHelper
{
public:
    /*
     * Recursive comparison used by unit tests: verifies that two states have
     * identical structure, texts, positions, expansion flags and selection.
     */
    static auto AreEqual(const TreeState& lhs, const TreeState& rhs) -> bool;
};
