#include <gtest/gtest.h>

#include "TreeExpansion.h"

namespace TreeExpansionTests
{
    TEST(SplitPath, EmptyPath)
    {
        EXPECT_TRUE(TreeExpansionHelper::SplitPath(L"").empty());
    }

    TEST(SplitPath, SingleKey)
    {
        auto keys = TreeExpansionHelper::SplitPath(L"root");
        ASSERT_EQ(keys.size(), 1u);
        EXPECT_EQ(keys[0], L"root");
    }

    TEST(SplitPath, MultipleKeys)
    {
        auto keys = TreeExpansionHelper::SplitPath(L"root.child.[0].name");
        ASSERT_EQ(keys.size(), 4u);
        EXPECT_EQ(keys[0], L"root");
        EXPECT_EQ(keys[1], L"child");
        EXPECT_EQ(keys[2], L"[0]");
        EXPECT_EQ(keys[3], L"name");
    }

    TEST(SplitPath, TrailingDotDropped)
    {
        auto keys = TreeExpansionHelper::SplitPath(L"root.child.");
        ASSERT_EQ(keys.size(), 2u);
        EXPECT_EQ(keys[1], L"child");
    }

    TEST(JoinPath, EmptyParents)
    {
        EXPECT_EQ(TreeExpansionHelper::JoinPath({}, L"root"), L"root");
    }

    TEST(JoinPath, Nested)
    {
        EXPECT_EQ(TreeExpansionHelper::JoinPath({ L"root", L"[0]" }, L"key"), L"root.[0].key");
    }

    TEST(MatchExpansion, EmptyOldState)
    {
        TreeExpansionState oldState;
        auto [toExpand, toSelect] = TreeExpansionHelper::MatchExpansion(oldState, { L"a", L"b" });
        EXPECT_TRUE(toExpand.empty());
        EXPECT_TRUE(toSelect.empty());
    }

    TEST(MatchExpansion, PathStillExists)
    {
        TreeExpansionState oldState;
        oldState.expandedPaths[L"root.child"] = true;
        oldState.expandedPaths[L"root.gone"]  = false;

        auto [toExpand, toSelect] = TreeExpansionHelper::MatchExpansion(oldState, { L"root", L"root.child", L"root.other" });
        ASSERT_EQ(toExpand.size(), 1u);
        EXPECT_EQ(toExpand[0], L"root.child");
        EXPECT_TRUE(toSelect.empty());
    }

    TEST(MatchExpansion, CollapsedPathsNotReExpanded)
    {
        TreeExpansionState oldState;
        oldState.expandedPaths[L"root.a"] = false;
        oldState.expandedPaths[L"root.b"] = true;

        auto [toExpand, toSelect] = TreeExpansionHelper::MatchExpansion(oldState, { L"root.a", L"root.b" });
        ASSERT_EQ(toExpand.size(), 1u);
        EXPECT_EQ(toExpand[0], L"root.b");
    }

    TEST(MatchExpansion, SelectionRestoredWhenExists)
    {
        TreeExpansionState oldState;
        oldState.selectedPath = { L"root", L"child" };

        auto [toExpand, toSelect] = TreeExpansionHelper::MatchExpansion(oldState, { L"root", L"root.child" });
        ASSERT_EQ(toSelect.size(), 2u);
        EXPECT_EQ(toSelect[0], L"root");
        EXPECT_EQ(toSelect[1], L"child");
    }

    TEST(MatchExpansion, SelectionDroppedWhenMissing)
    {
        TreeExpansionState oldState;
        oldState.selectedPath = { L"root", L"gone" };

        auto [toExpand, toSelect] = TreeExpansionHelper::MatchExpansion(oldState, { L"root", L"root.here" });
        EXPECT_TRUE(toSelect.empty());
    }

    TEST(MatchExpansion, EmptyNewTreeRestoresNothing)
    {
        TreeExpansionState oldState;
        oldState.expandedPaths[L"root.child"] = true;
        oldState.selectedPath                 = { L"root", L"child" };

        auto [toExpand, toSelect] = TreeExpansionHelper::MatchExpansion(oldState, {});
        EXPECT_TRUE(toExpand.empty());
        EXPECT_TRUE(toSelect.empty());
    }

    TEST(MatchExpansion, NewPathsNotInOldStateAreIgnored)
    {
        TreeExpansionState oldState;
        oldState.expandedPaths[L"root.child"] = true;

        // Nodes that appeared in the new document must stay collapsed
        auto [toExpand, toSelect] = TreeExpansionHelper::MatchExpansion(oldState, { L"root", L"root.fresh" });
        EXPECT_TRUE(toExpand.empty());
        EXPECT_TRUE(toSelect.empty());
    }

    TEST(MatchExpansion, PreservesOrderOfNewPaths)
    {
        TreeExpansionState oldState;
        oldState.expandedPaths[L"a"] = true;
        oldState.expandedPaths[L"b"] = true;
        oldState.expandedPaths[L"c"] = true;

        // Follows the order of newPaths, not the hash order of expandedPaths
        auto [toExpand, toSelect] = TreeExpansionHelper::MatchExpansion(oldState, { L"c", L"a", L"b" });
        ASSERT_EQ(toExpand.size(), 3u);
        EXPECT_EQ(toExpand[0], L"c");
        EXPECT_EQ(toExpand[1], L"a");
        EXPECT_EQ(toExpand[2], L"b");
    }
}    // namespace TreeExpansionTests
