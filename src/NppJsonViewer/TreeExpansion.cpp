#include "TreeExpansion.h"

#include <algorithm>

auto TreeExpansionHelper::SplitPath(const std::wstring& path) -> std::vector<std::wstring>
{
    std::vector<std::wstring> keys;

    if (path.empty())
        return keys;

    size_t start = 0;
    while (start <= path.size())
    {
        auto pos = path.find(L'.', start);
        if (pos == std::wstring::npos)
        {
            keys.emplace_back(path.substr(start));
            break;
        }

        keys.emplace_back(path.substr(start, pos - start));
        start = pos + 1;
    }

    // An empty trailing key (e.g. a path ending with a dot) is dropped
    if (!keys.empty() && keys.back().empty())
        keys.pop_back();

    return keys;
}

auto TreeExpansionHelper::JoinPath(const std::vector<std::wstring>& parentKeys, const std::wstring& key) -> std::wstring
{
    std::wstring path;
    for (const auto& part : parentKeys)
    {
        path += part;
        path += L'.';
    }
    path += key;
    return path;
}

auto TreeExpansionHelper::MatchExpansion(const TreeExpansionState& oldState,
                                         const std::vector<std::wstring>& newPaths)
    -> std::pair<std::vector<std::wstring>, std::vector<std::wstring>>
{
    std::vector<std::wstring> pathsToExpand;

    for (const auto& path : newPaths)
    {
        auto find = oldState.expandedPaths.find(path);
        if (find != oldState.expandedPaths.cend() && find->second)
            pathsToExpand.push_back(path);
    }

    std::vector<std::wstring> pathToSelect;
    if (!oldState.selectedPath.empty())
    {
        // Reconstruct the selected path in "joined" form and check existence
        std::wstring joined;
        for (const auto& key : oldState.selectedPath)
        {
            if (!joined.empty())
                joined += L'.';
            joined += key;
        }

        if (std::find(newPaths.cbegin(), newPaths.cend(), joined) != newPaths.cend())
            pathToSelect = oldState.selectedPath;
    }

    return { pathsToExpand, pathToSelect };
}
