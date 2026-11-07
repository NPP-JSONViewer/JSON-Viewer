#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

/*
 * TreeExpansionState captures which nodes of the JSON tree are expanded and
 * which one is selected, keyed by node path.
 *
 * It exists so that the state can be re-applied onto a freshly built tree.
 * The typical case is "Refresh JSON Tree", which rebuilds every node and
 * therefore loses the expansion the user set up. Paths that no longer exist in
 * the new tree are simply dropped.
 *
 * A path is the list of node keys from the tree root down to a node, joined
 * with '.' (e.g. "root.child", "root.[0].[1].key"). The tree root itself is not
 * part of the path.
 */
struct TreeExpansionState
{
    std::unordered_map<std::wstring, bool> expandedPaths;    // path -> was expanded
    std::vector<std::wstring>              selectedPath;     // key-path of the selected node, empty when none
};

class TreeExpansionHelper
{
public:
    // Split a node path into its keys: "root.[0].key" -> { "root", "[0]", "key" }
    static auto SplitPath(const std::wstring& path) -> std::vector<std::wstring>;

    // Build the path of a node from the path of its parent and the node key.
    static auto JoinPath(const std::vector<std::wstring>& parentKeys, const std::wstring& key) -> std::wstring;

    /*
     * Compute which paths of `oldState` must be re-expanded onto a new tree
     * whose node paths are listed in `newPaths`, and where the selection has to
     * be restored (only if that path still exists).
     *
     * Duplicate keys resolve to the first matching node (accepted trade-off:
     * JSON object keys are unique in practice, and making this exact would
     * require disambiguating sibling order as well).
     */
    static auto MatchExpansion(const TreeExpansionState& oldState,
                               const std::vector<std::wstring>& newPaths)
        -> std::pair<std::vector<std::wstring> /*pathsToExpand*/, std::vector<std::wstring> /*pathToSelect*/>;
};
