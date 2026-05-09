#include "CashRegister.hpp"

#include <limits>
#include <vector>

CashRegister::CashRegister(std::map<int, int> initialCoins) : coins(std::move(initialCoins)) {}

std::optional<std::map<int, int>> CashRegister::makeChange(int amount) const {
    if (amount < 0) return std::nullopt;
    if (amount == 0) return std::map<int, int>{};

    // Flatten the drawer into a list of individual coins
    std::vector<int> availableCoins;
    for (const auto& [denom, count] : coins) {
        if (denom <= 0 || count <= 0) continue;
        for (int i = 0; i < count; i++) availableCoins.push_back(denom);
    }

    const int inf = std::numeric_limits<int>::max();
    std::vector<int> minCoinsFor(amount + 1, inf);
    std::vector<int> lastCoinUsed(amount + 1, 0);
    minCoinsFor[0] = 0;

    // 0/1 knapsack
    for (int coin : availableCoins) {
        for (int v = amount; v >= coin; --v) {
            if (minCoinsFor[v - coin] != inf && minCoinsFor[v - coin] + 1 < minCoinsFor[v]) {
                minCoinsFor[v] = minCoinsFor[v - coin] + 1;
                lastCoinUsed[v] = coin;
            }
        }
    }

    if (minCoinsFor[amount] == inf) return std::nullopt;

    std::map<int, int> result;
    for (int v = amount; v > 0; ) {
        int d = lastCoinUsed[v];
        ++result[d];
        v -= d;
    }
    return result;
}
