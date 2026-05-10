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

void CashRegister::deposit(const std::vector<int>& coinsIn) {
    for (int c : coinsIn) coins[c]++;
}

void CashRegister::withdraw(const std::map<int, int>& coinsOut) {
    for (const auto& [denom, count] : coinsOut) coins[denom] -= count;
}

std::optional<std::map<int, int>> CashRegister::acceptPayment(const std::vector<int>& inserted, int price) {
    if (price < 0) return std::nullopt;

    int total = 0;
    for (int c : inserted) total += c;
    if (total < price) return std::nullopt;

    deposit(inserted);
    auto change = makeChange(total - price);
    if (!change) {
        for (int c : inserted) coins[c]--;
        return std::nullopt;
    }
    withdraw(*change);
    return change;
}

void CashRegister::refund(const std::vector<int>& inserted, const std::map<int, int>& change) {
    for (int c : inserted) coins[c]--;
    for (const auto& [denom, count] : change) coins[denom] += count;
}
