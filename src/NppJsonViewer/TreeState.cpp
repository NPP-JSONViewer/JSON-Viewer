#include "TreeState.h"

namespace
{
    auto AreNodesEqual(const TreeStateNode& lhs, const TreeStateNode& rhs) -> bool
    {
        if (lhs.text != rhs.text || lhs.expanded != rhs.expanded)
            return false;

        if (lhs.pos.has_value() != rhs.pos.has_value())
            return false;

        if (lhs.pos.has_value())
        {
            if (lhs.pos->nLine != rhs.pos->nLine || lhs.pos->nColumn != rhs.pos->nColumn
                || lhs.pos->nKeyLength != rhs.pos->nKeyLength)
                return false;
        }

        if (lhs.children.size() != rhs.children.size())
            return false;

        for (size_t i = 0; i < lhs.children.size(); ++i)
        {
            if (!AreNodesEqual(lhs.children[i], rhs.children[i]))
                return false;
        }

        return true;
    }
}

auto TreeStateHelper::AreEqual(const TreeState& lhs, const TreeState& rhs) -> bool
{
    if (lhs.selectedPath != rhs.selectedPath)
        return false;

    if (lhs.roots.size() != rhs.roots.size())
        return false;

    for (size_t i = 0; i < lhs.roots.size(); ++i)
    {
        if (!AreNodesEqual(lhs.roots[i], rhs.roots[i]))
            return false;
    }

    return true;
}
