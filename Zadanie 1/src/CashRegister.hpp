#pragma once

#include <map>
#include <optional>
#include <vector>

// All amounts are in grosze (1 PLN = 100 gr).
class CashRegister {
public:
    explicit CashRegister(std::map<int, int> initialCoins);

    // Returns the coins to give as change, or nullopt if it's impossible to make the change.
    std::optional<std::map<int, int>> makeChange(int amount) const;

    // Returns the change to give back, or nullopt if the payment is invalid.
    std::optional<std::map<int, int>> acceptPayment(const std::vector<int>& inserted, int price);

    const std::map<int, int>& getCoins() const { return coins; }

private:
    std::map<int, int> coins;
};
