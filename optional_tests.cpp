#include <gtest/gtest.h>
#include <string>
#include "optional.h"
#include "tracked.h"

// =============================================================================
//  Optional<T> tests
// =============================================================================

TEST(OptionalBasic, DefaultEmpty) {
    Optional<int> o;
    EXPECT_FALSE(o.has_value());
    EXPECT_FALSE(static_cast<bool>(o));
}

TEST(OptionalBasic, ConstructWithValue) {
    Optional<int> o(42);
    EXPECT_TRUE(o.has_value());
    EXPECT_EQ(o.value(), 42);
}

TEST(OptionalBasic, ConstructWithRvalue) {
    std::string s = "hello";
    Optional<std::string> o(std::move(s));
    EXPECT_TRUE(o.has_value());
    EXPECT_EQ(o.value(), "hello");
}

TEST(OptionalAccess, Value) {
    Optional<int> o(99);
    EXPECT_EQ(o.value(), 99);
    o.value() = 100;
    EXPECT_EQ(o.value(), 100);
}

TEST(OptionalAccess, ConstValue) {
    const Optional<int> o(42);
    EXPECT_EQ(o.value(), 42);
}

TEST(OptionalAccess, ValueOr) {
    Optional<int> filled(42);
    Optional<int> empty;
    EXPECT_EQ(filled.value_or(0), 42);
    EXPECT_EQ(empty.value_or(0), 0);
}

TEST(OptionalAccess, Dereference) {
    Optional<int> o(42);
    EXPECT_EQ(*o, 42);
    *o = 100;
    EXPECT_EQ(*o, 100);
}

TEST(OptionalAccess, Arrow) {
    Optional<std::string> o(std::string("hello"));
    EXPECT_EQ(o->size(), 5u);
}

TEST(OptionalAccess, ConstArrow) {
    const Optional<std::string> o(std::string("hello"));
    EXPECT_EQ(o->size(), 5u);
}

TEST(OptionalCopy, CopyConstructorEmpty) {
    Optional<int> a;
    Optional<int> b(a);
    EXPECT_FALSE(b.has_value());
}

TEST(OptionalCopy, CopyConstructorFilled) {
    Optional<int> a(42);
    Optional<int> b(a);
    EXPECT_TRUE(b.has_value());
    EXPECT_EQ(b.value(), 42);

    b.value() = 100;
    EXPECT_EQ(a.value(), 42);
}

TEST(OptionalCopy, CopyAssignmentFilledToEmpty) {
    Optional<int> a(42);
    Optional<int> b;
    b = a;
    EXPECT_TRUE(b.has_value());
    EXPECT_EQ(b.value(), 42);
}

TEST(OptionalCopy, CopyAssignmentEmptyToFilled) {
    Optional<int> a;
    Optional<int> b(42);
    b = a;
    EXPECT_FALSE(b.has_value());
}

TEST(OptionalCopy, CopyAssignmentFilledToFilled) {
    Optional<int> a(42);
    Optional<int> b(99);
    b = a;
    EXPECT_EQ(b.value(), 42);
}

TEST(OptionalCopy, SelfAssignment) {
    Optional<int> a(42);
    a = a;
    EXPECT_TRUE(a.has_value());
    EXPECT_EQ(a.value(), 42);
}

TEST(OptionalMove, MoveConstructor) {
    Optional<std::string> a(std::string("hello"));
    Optional<std::string> b(std::move(a));
    EXPECT_TRUE(b.has_value());
    EXPECT_EQ(b.value(), "hello");
}

TEST(OptionalMove, MoveAssignment) {
    Optional<std::string> a(std::string("hello"));
    Optional<std::string> b;
    b = std::move(a);
    EXPECT_TRUE(b.has_value());
    EXPECT_EQ(b.value(), "hello");
}

TEST(OptionalAssignValue, FromLvalue) {
    Optional<int> o;
    int x = 42;
    o = x;
    EXPECT_TRUE(o.has_value());
    EXPECT_EQ(o.value(), 42);
}

TEST(OptionalAssignValue, FromRvalue) {
    Optional<std::string> o;
    o = std::string("world");
    EXPECT_TRUE(o.has_value());
    EXPECT_EQ(o.value(), "world");
}

TEST(OptionalAssignValue, OverwriteExisting) {
    Optional<int> o(42);
    o = 99;
    EXPECT_EQ(o.value(), 99);
}

TEST(OptionalModifiers, Reset) {
    Optional<int> o(42);
    o.reset();
    EXPECT_FALSE(o.has_value());
}

TEST(OptionalModifiers, ResetEmpty) {
    Optional<int> o;
    o.reset();
    EXPECT_FALSE(o.has_value());
}

TEST(OptionalModifiers, Emplace) {
    Optional<std::string> o;
    o.emplace(5, 'x');
    EXPECT_TRUE(o.has_value());
    EXPECT_EQ(o.value(), "xxxxx");
}

TEST(OptionalModifiers, EmplaceOverwrite) {
    Optional<std::string> o(std::string("old"));
    o.emplace(3, 'y');
    EXPECT_EQ(o.value(), "yyy");
}

TEST(OptionalModifiers, Swap) {
    Optional<int> a(42);
    Optional<int> b;
    a.swap(b);
    EXPECT_FALSE(a.has_value());
    EXPECT_TRUE(b.has_value());
    EXPECT_EQ(b.value(), 42);
}

TEST(OptionalModifiers, SwapBothFilled) {
    Optional<int> a(1);
    Optional<int> b(2);
    a.swap(b);
    EXPECT_EQ(a.value(), 2);
    EXPECT_EQ(b.value(), 1);
}

TEST(OptionalModifiers, SwapBothEmpty) {
    Optional<int> a;
    Optional<int> b;
    a.swap(b);
    EXPECT_FALSE(a.has_value());
    EXPECT_FALSE(b.has_value());
}

TEST(OptionalComparison, BothEmpty) {
    Optional<int> a;
    Optional<int> b;
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST(OptionalComparison, BothFilledEqual) {
    Optional<int> a(42);
    Optional<int> b(42);
    EXPECT_TRUE(a == b);
}

TEST(OptionalComparison, BothFilledNotEqual) {
    Optional<int> a(42);
    Optional<int> b(99);
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TEST(OptionalComparison, OneEmptyOneFilled) {
    Optional<int> a(42);
    Optional<int> b;
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TEST_F(TrackedTest, OptionalDestructor) {
    {
        Optional<Tracked> o(Tracked(10));
        EXPECT_EQ(o.value().value, 10);
    }
}

TEST_F(TrackedTest, OptionalResetDestroys) {
    {
        Optional<Tracked> o(Tracked(10));
        o.reset();
        EXPECT_EQ(alive_count, 0);
    }
}

TEST_F(TrackedTest, OptionalEmplaceDestroysPrevious) {
    {
        Optional<Tracked> o(Tracked(10));
        o.emplace(20);
        EXPECT_EQ(o.value().value, 20);
    }
}

TEST_F(TrackedTest, OptionalCopyNoLeak) {
    {
        Optional<Tracked> a(Tracked(5));
        Optional<Tracked> b(a);
        EXPECT_EQ(b.value().value, 5);
    }
}

TEST_F(TrackedTest, OptionalMoveNoLeak) {
    {
        Optional<Tracked> a(Tracked(5));
        Optional<Tracked> b(std::move(a));
    }
}

TEST_F(TrackedTest, OptionalAssignmentNoLeak) {
    {
        Optional<Tracked> a(Tracked(1));
        Optional<Tracked> b(Tracked(2));
        b = a;
    }
}
