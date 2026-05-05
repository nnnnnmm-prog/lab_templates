#include <gtest/gtest.h>
#include <string>
#include "array.h"
#include "tracked.h"

// =============================================================================
//  Array<T> tests
// =============================================================================

TEST(ArrayBasic, DefaultConstructor) {
    Array<int> a;
    EXPECT_EQ(a.size(), 0u);
    EXPECT_TRUE(a.empty());
    EXPECT_EQ(a.data(), nullptr);
}

TEST(ArrayBasic, SizeConstructor) {
    Array<int> a(5);
    EXPECT_EQ(a.size(), 5u);
    EXPECT_FALSE(a.empty());
    EXPECT_NE(a.data(), nullptr);
}

TEST(ArrayBasic, SizeValueConstructor) {
    Array<int> a(4, 42);
    EXPECT_EQ(a.size(), 4u);
    for (size_t i = 0; i < a.size(); ++i) {
        EXPECT_EQ(a[i], 42);
    }
}

TEST(ArrayBasic, SizeZero) {
    Array<int> a(0);
    EXPECT_EQ(a.size(), 0u);
    EXPECT_TRUE(a.empty());
}

TEST(ArrayAccess, Subscript) {
    Array<int> a(3, 0);
    a[0] = 10;
    a[1] = 20;
    a[2] = 30;
    EXPECT_EQ(a[0], 10);
    EXPECT_EQ(a[1], 20);
    EXPECT_EQ(a[2], 30);
}

TEST(ArrayAccess, ConstSubscript) {
    Array<int> a(3, 99);
    const Array<int>& ref = a;
    EXPECT_EQ(ref[0], 99);
    EXPECT_EQ(ref[1], 99);
    EXPECT_EQ(ref[2], 99);
}

TEST(ArrayAccess, FrontBack) {
    Array<int> a(3, 0);
    a[0] = 1;
    a[2] = 3;
    EXPECT_EQ(a.front(), 1);
    EXPECT_EQ(a.back(), 3);
}

TEST(ArrayAccess, ConstFrontBack) {
    Array<int> a(2, 55);
    const Array<int>& ref = a;
    EXPECT_EQ(ref.front(), 55);
    EXPECT_EQ(ref.back(), 55);
}

TEST(ArrayAccess, Data) {
    Array<int> a(3, 7);
    int* p = a.data();
    EXPECT_EQ(p[0], 7);
    p[1] = 100;
    EXPECT_EQ(a[1], 100);
}

TEST(ArrayCopy, CopyConstructor) {
    Array<int> a(3, 42);
    Array<int> b(a);
    EXPECT_EQ(b.size(), 3u);
    EXPECT_EQ(b[0], 42);
    EXPECT_EQ(b[1], 42);
    EXPECT_EQ(b[2], 42);

    // deep copy: modifying b should not affect a
    b[0] = 999;
    EXPECT_EQ(a[0], 42);
}

TEST(ArrayCopy, CopyAssignment) {
    Array<int> a(3, 10);
    Array<int> b(2, 20);
    b = a;
    EXPECT_EQ(b.size(), 3u);
    EXPECT_EQ(b[0], 10);

    b[0] = 999;
    EXPECT_EQ(a[0], 10);
}

TEST(ArrayCopy, SelfAssignment) {
    Array<int> a(3, 42);
    a = a;
    EXPECT_EQ(a.size(), 3u);
    EXPECT_EQ(a[0], 42);
}

TEST(ArrayMove, MoveConstructor) {
    Array<int> a(3, 42);
    Array<int> b(std::move(a));
    EXPECT_EQ(b.size(), 3u);
    EXPECT_EQ(b[0], 42);
    EXPECT_EQ(a.size(), 0u);
    EXPECT_EQ(a.data(), nullptr);
}

TEST(ArrayMove, MoveAssignment) {
    Array<int> a(3, 10);
    Array<int> b(2, 20);
    b = std::move(a);
    EXPECT_EQ(b.size(), 3u);
    EXPECT_EQ(b[0], 10);
    EXPECT_EQ(a.size(), 0u);
    EXPECT_EQ(a.data(), nullptr);
}

TEST(ArrayOps, Fill) {
    Array<int> a(5, 0);
    a.fill(77);
    for (size_t i = 0; i < a.size(); ++i) {
        EXPECT_EQ(a[i], 77);
    }
}

TEST(ArrayOps, Swap) {
    Array<int> a(3, 1);
    Array<int> b(5, 2);
    a.swap(b);
    EXPECT_EQ(a.size(), 5u);
    EXPECT_EQ(a[0], 2);
    EXPECT_EQ(b.size(), 3u);
    EXPECT_EQ(b[0], 1);
}

TEST(ArrayComparison, Equal) {
    Array<int> a(3, 42);
    Array<int> b(3, 42);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST(ArrayComparison, NotEqualValues) {
    Array<int> a(3, 42);
    Array<int> b(3, 43);
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TEST(ArrayComparison, NotEqualSizes) {
    Array<int> a(3, 42);
    Array<int> b(4, 42);
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TEST(ArrayComparison, BothEmpty) {
    Array<int> a;
    Array<int> b;
    EXPECT_TRUE(a == b);
}

TEST(ArrayString, WorksWithStrings) {
    Array<std::string> a(3, "hello");
    EXPECT_EQ(a[0], "hello");
    EXPECT_EQ(a.size(), 3u);

    Array<std::string> b(a);
    b[0] = "world";
    EXPECT_EQ(a[0], "hello");
    EXPECT_EQ(b[0], "world");
}

TEST_F(TrackedTest, ArrayDestructor) {
    {
        Array<Tracked> a(5, Tracked(10));
    }
}

TEST_F(TrackedTest, ArrayCopyDeep) {
    {
        Array<Tracked> a(3, Tracked(7));
        Array<Tracked> b(a);
        EXPECT_EQ(b[0].value, 7);
    }
}

TEST_F(TrackedTest, ArrayMoveNoLeak) {
    {
        Array<Tracked> a(3, Tracked(5));
        Array<Tracked> b(std::move(a));
        EXPECT_EQ(b[0].value, 5);
    }
}

TEST_F(TrackedTest, ArrayReassignNoLeak) {
    {
        Array<Tracked> a(3, Tracked(1));
        Array<Tracked> b(4, Tracked(2));
        b = a;
        EXPECT_EQ(b.size(), 3u);
    }
}
