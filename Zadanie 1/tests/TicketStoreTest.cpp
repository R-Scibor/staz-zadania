#include "TicketStore.hpp"

#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

TEST(TicketStore, ReserveDecrementsInventory) {
    TicketStore s({{"normal", 2}}, 60s);
    auto t0 = TicketStore::clock::now();
    auto id = s.reserve("normal", t0);
    ASSERT_TRUE(id.has_value());
    EXPECT_EQ(s.getAvailable()["normal"], 1);
}

TEST(TicketStore, ReserveFailsWhenSoldOut) {
    TicketStore s({{"normal", 0}}, 60s);
    auto t0 = TicketStore::clock::now();
    EXPECT_FALSE(s.reserve("normal", t0).has_value());
}

TEST(TicketStore, CancelReturnsToPool) {
    TicketStore s({{"normal", 1}}, 60s);
    auto t0 = TicketStore::clock::now();
    auto id = s.reserve("normal", t0);
    ASSERT_TRUE(id.has_value());
    EXPECT_TRUE(s.cancel(*id));
    EXPECT_EQ(s.getAvailable()["normal"], 1);
}

TEST(TicketStore, FinalizeProducesSequentialTicketNumbers) {
    TicketStore s({{"normal", 2}}, 60s);
    auto t0 = TicketStore::clock::now();
    auto a = s.reserve("normal", t0);
    auto b = s.reserve("normal", t0);
    auto numA = s.finalize(*a, t0);
    auto numB = s.finalize(*b, t0);
    ASSERT_TRUE(numA.has_value() && numB.has_value());
    EXPECT_NE(*numA, *numB);
    EXPECT_EQ(s.getAvailable()["normal"], 0);
}

TEST(TicketStore, ExpiredReservationCannotBeFinalized) {
    TicketStore s({{"normal", 1}}, 1s);
    auto t0 = TicketStore::clock::now();
    auto id = s.reserve("normal", t0);
    ASSERT_TRUE(id.has_value());
    EXPECT_FALSE(s.finalize(*id, t0 + 2s).has_value());
    EXPECT_EQ(s.getAvailable()["normal"], 1);
}

TEST(TicketStore, ReleaseExpiredReturnsTicketsToPool) {
    TicketStore s({{"normal", 2}}, 1s);
    auto t0 = TicketStore::clock::now();
    s.reserve("normal", t0);
    s.reserve("normal", t0);
    EXPECT_EQ(s.releaseExpired(t0 + 2s), 2);
    EXPECT_EQ(s.getAvailable()["normal"], 2);
}

// Spec example 3: two automatons race for the last ticket. Exactly one wins.
TEST(TicketStore, ConcurrentReservesOfLastTicket) {
    TicketStore s({{"normal", 1}}, 60s);
    auto t0 = TicketStore::clock::now();
    std::vector<std::thread> threads;
    std::atomic<int> winners{0};
    for (int i = 0; i < 16; i++) {
        threads.emplace_back([&] {
            if (s.reserve("normal", t0).has_value()) winners++;
        });
    }
    for (auto& t : threads) t.join();
    EXPECT_EQ(winners.load(), 1);
    EXPECT_EQ(s.getAvailable()["normal"], 0);
}
