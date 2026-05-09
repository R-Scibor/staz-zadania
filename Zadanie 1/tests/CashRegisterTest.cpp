#include "CashRegister.hpp"

#include <gtest/gtest.h>

#include <map>

static int totalCoins(const std::map<int, int>& m) {
    int n = 0;
    for (const auto& [_, c] : m) n += c;
    return n;
}

static int totalValue(const std::map<int, int>& m) {
    int v = 0;
    for (const auto& [d, c] : m) v += d * c;
    return v;
}

TEST(CashRegister, ZeroAmountReturnsEmpty) {
    CashRegister r({{100, 5}});
    auto change = r.makeChange(0);
    ASSERT_TRUE(change.has_value());
    EXPECT_TRUE(change->empty());
}

TEST(CashRegister, NegativeAmountFails) {
    CashRegister r({{100, 5}});
    EXPECT_FALSE(r.makeChange(-50).has_value());
}

TEST(CashRegister, EmptyDrawerCannotMakeChange) {
    CashRegister r({});
    EXPECT_FALSE(r.makeChange(100).has_value());
}

TEST(CashRegister, DoesNotMutateDrawer) {
    std::map<int, int> initial{{100, 5}, {50, 2}};
    CashRegister r(initial);
    (void)r.makeChange(150);
    EXPECT_EQ(r.getCoins(), initial);
}

// Spec example 1: 1.50 zl change from {1x 2zl, 5x 1zl, 2x 0.50zl}
// expected: 1x 1zl + 1x 0.50zl = 2 coins (minimum).
TEST(CashRegister, SpecExample1_OneFiftyChange) {
    CashRegister r({{200, 1}, {100, 5}, {50, 2}});
    auto change = r.makeChange(150);
    ASSERT_TRUE(change.has_value());
    EXPECT_EQ(totalValue(*change), 150);
    EXPECT_EQ(totalCoins(*change), 2);
}

// Spec example 4: 2.20 zl change but drawer has only 2zl coins.
TEST(CashRegister, SpecExample4_ImpossibleChange) {
    CashRegister r({{200, 10}});
    EXPECT_FALSE(r.makeChange(220).has_value());
}

// Drawer: 1x 50gr, 4x 20gr. Target: 60gr.
// Greedy by largest denom: 50 + ??? (need 10gr, none) -> fail.
// DP: 3x 20gr = 60gr, 3 coins. Should succeed.
TEST(CashRegister, ConstrainedSupplyDefeatGreedy) {
    CashRegister r({{50, 1}, {20, 4}});
    auto change = r.makeChange(60);
    ASSERT_TRUE(change.has_value());
    EXPECT_EQ(totalValue(*change), 60);
    EXPECT_EQ((*change)[20], 3);
    EXPECT_EQ((*change).count(50), 0u);
}

// Min-coins must be minimum, not any valid combination.
TEST(CashRegister, PrefersFewerCoins) {
    CashRegister r({{100, 1}, {10, 20}});
    auto change = r.makeChange(100);
    ASSERT_TRUE(change.has_value());
    EXPECT_EQ(totalCoins(*change), 1);
    EXPECT_EQ((*change)[100], 1);
}
