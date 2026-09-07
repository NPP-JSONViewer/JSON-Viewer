#include <gtest/gtest.h>

#include "TreeState.h"

namespace TreeStateTests
{
    TEST(AreEqual, EmptyStates)
    {
        TreeState a, b;
        EXPECT_TRUE(TreeStateHelper::AreEqual(a, b));
    }

    TEST(AreEqual, IdenticalStates)
    {
        TreeStateNode child;
        child.text = L"a : 1";

        TreeStateNode parent;
        parent.text     = L"obj {1}";
        parent.expanded = true;
        parent.children = {child};

        TreeState a, b;
        a.roots.push_back(parent);
        b.roots.push_back(parent);
        a.selectedPath = {L"obj", L"a"};
        b.selectedPath = {L"obj", L"a"};

        EXPECT_TRUE(TreeStateHelper::AreEqual(a, b));
    }

    TEST(AreEqual, DifferentText)
    {
        TreeState a, b;
        TreeStateNode n;
        n.text = L"key";
        a.roots.push_back(n);
        n.text = L"other";
        b.roots.push_back(n);

        EXPECT_FALSE(TreeStateHelper::AreEqual(a, b));
    }

    TEST(AreEqual, DifferentExpansion)
    {
        TreeState a, b;
        TreeStateNode n;
        n.text     = L"key";
        n.expanded = true;
        a.roots.push_back(n);
        n.expanded = false;
        b.roots.push_back(n);

        EXPECT_FALSE(TreeStateHelper::AreEqual(a, b));
    }

    TEST(AreEqual, DifferentSelection)
    {
        TreeState a, b;
        TreeStateNode n;
        n.text = L"key";
        a.roots.push_back(n);
        b.roots.push_back(n);
        a.selectedPath = {L"key"};
        b.selectedPath = {L"other"};

        EXPECT_FALSE(TreeStateHelper::AreEqual(a, b));
    }

    TEST(AreEqual, DifferentRootCount)
    {
        TreeState a, b;
        TreeStateNode n;
        n.text = L"key";
        a.roots.push_back(n);
        b.roots.push_back(n);
        b.roots.push_back(n);

        EXPECT_FALSE(TreeStateHelper::AreEqual(a, b));
    }

    TEST(AreEqual, DifferentChildrenCount)
    {
        TreeState a, b;
        TreeStateNode parent;
        parent.text = L"obj {2}";
        TreeStateNode c1, c2;
        c1.text = L"a";
        c2.text = L"b";
        parent.children = {c1, c2};
        a.roots.push_back(parent);

        parent.children = {c1};
        b.roots.push_back(parent);

        EXPECT_FALSE(TreeStateHelper::AreEqual(a, b));
    }

    TEST(AreEqual, PositionCompared)
    {
        TreeState a, b;
        TreeStateNode n;
        n.text = L"key";
        n.pos  = Position{3, 5, 4};
        a.roots.push_back(n);

        TreeStateNode m;
        m.text = L"key";
        m.pos  = Position{3, 5, 4};
        b.roots.push_back(m);

        EXPECT_TRUE(TreeStateHelper::AreEqual(a, b));

        m.pos = Position{4, 5, 4};
        b.roots.clear();
        b.roots.push_back(m);
        EXPECT_FALSE(TreeStateHelper::AreEqual(a, b));
    }

    TEST(AreEqual, PositionPresenceCompared)
    {
        TreeState a, b;
        TreeStateNode n;
        n.text = L"key";
        n.pos  = Position{1, 2, 3};
        a.roots.push_back(n);

        TreeStateNode m;
        m.text = L"key";    // no position at all
        b.roots.push_back(m);

        EXPECT_FALSE(TreeStateHelper::AreEqual(a, b));
    }
}
