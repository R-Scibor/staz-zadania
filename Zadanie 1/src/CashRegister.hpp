#pragma once

#include <map>
#include <optional>
#include <vector>

// All amounts are in grosze (1 PLN = 100 gr).
class CashRegister {
public:
    explicit CashRegister(std::map<int, int> initialCoins);

    std::optional<std::map<int, int>> makeChange(int amount) const;

    void deposit(const std::vector<int>& coinsIn);
    void withdraw(const std::map<int, int>& coinsOut);

    std::optional<std::map<int, int>> acceptPayment(const std::vector<int>& inserted, int price);
    void refund(const std::vector<int>& inserted, const std::map<int, int>& change);

    const std::map<int, int>& getCoins() const { return coins; }

private:
    std::map<int, int> coins;
};
