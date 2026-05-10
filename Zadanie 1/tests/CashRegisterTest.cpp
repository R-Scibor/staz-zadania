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
// DP: 3x 20gr = 60gr, 3 coins.
TEST(CashRegister, ConstrainedSupplyDefeatGreedy) {
    CashRegister r({{50, 1}, {20, 4}});
    auto change = r.makeChange(60);
    ASSERT_TRUE(change.has_value());
    EXPECT_EQ(totalValue(*change), 60);
    EXPECT_EQ((*change)[20], 3);
    EXPECT_EQ((*change).count(50), 0u);
}

TEST(CashRegister, PrefersFewerCoins) {
    CashRegister r({{100, 1}, {10, 20}});
    auto change = r.makeChange(100);
    ASSERT_TRUE(change.has_value());
    EXPECT_EQ(totalCoins(*change), 1);
    EXPECT_EQ((*change)[100], 1);
}

TEST(CashRegister, AcceptPaymentExactNoChange) {
    CashRegister r({{50, 1}});
    auto change = r.acceptPayment({100, 50}, 150);
    ASSERT_TRUE(change.has_value());
    EXPECT_TRUE(change->empty());
    EXPECT_EQ(r.getCoins().at(100), 1);
    EXPECT_EQ(r.getCoins().at(50), 2);
}

TEST(CashRegister, AcceptPaymentReturnsChangeAndUpdatesDrawer) {
    CashRegister r({{50, 1}});
    auto change = r.acceptPayment({200}, 150);
    ASSERT_TRUE(change.has_value());
    EXPECT_EQ(totalValue(*change), 50);
    EXPECT_EQ((*change)[50], 1);
    EXPECT_EQ(r.getCoins().at(200), 1);
    EXPECT_EQ(r.getCoins().at(50), 0);
}

TEST(CashRegister, AcceptPaymentInsufficientLeavesDrawerUnchanged) {
    std::map<int, int> initial{{100, 5}, {50, 2}};
    CashRegister r(initial);
    auto change = r.acceptPayment({100}, 150);
    EXPECT_FALSE(change.has_value());
    EXPECT_EQ(r.getCoins(), initial);
}

TEST(CashRegister, AcceptPaymentCantMakeChangeLeavesDrawerUnchanged) {
    std::map<int, int> initial{{200, 2}};
    CashRegister r(initial);
    auto change = r.acceptPayment({200, 200}, 350);
    EXPECT_FALSE(change.has_value());
    EXPECT_EQ(r.getCoins(), initial);
}

TEST(CashRegister, RefundUndoesAcceptPayment) {
    CashRegister r({{50, 1}, {100, 2}});
    auto change = r.acceptPayment({200}, 150);
    ASSERT_TRUE(change.has_value());
    r.refund({200}, *change);
    EXPECT_EQ(r.getCoins().at(50), 1);
    EXPECT_EQ(r.getCoins().at(100), 2);
    EXPECT_EQ(r.getCoins().at(200), 0);
}
