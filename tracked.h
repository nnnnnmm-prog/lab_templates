#pragma once

#include <gtest/gtest.h>

inline int alive_count = 0;

struct Tracked {
    int value;

    Tracked() : value(0)            { ++alive_count; }
    Tracked(int v) : value(v)       { ++alive_count; }
    Tracked(const Tracked& o) : value(o.value) { ++alive_count; }
    Tracked(Tracked&& o) : value(o.value) { o.value = -1; ++alive_count; }

    Tracked& operator=(const Tracked& o) { value = o.value; return *this; }
    Tracked& operator=(Tracked&& o) { value = o.value; o.value = -1; return *this; }

    ~Tracked() { --alive_count; }

    bool operator==(const Tracked& o) const { return value == o.value; }
    bool operator!=(const Tracked& o) const { return value != o.value; }
};

class TrackedTest : public ::testing::Test {
protected:
    void SetUp() override { alive_count = 0; }
    void TearDown() override { EXPECT_EQ(alive_count, 0) << "Memory leak: not all objects destroyed"; }
};
