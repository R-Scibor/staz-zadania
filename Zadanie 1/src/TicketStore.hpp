#pragma once

#include <chrono>
#include <map>
#include <mutex>
#include <optional>
#include <string>

class TicketStore {
public:
    using clock = std::chrono::steady_clock;

    TicketStore(std::map<std::string, int> initialInventory, std::chrono::seconds reservationTtl);

    std::map<std::string, int> getAvailable() const;

    std::optional<int> reserve(const std::string& type, clock::time_point now);

    bool cancel(int reservationId);

    std::optional<int> finalize(int reservationId, clock::time_point now);

    int releaseExpired(clock::time_point now);

private:
    struct Reservation {
        std::string type;
        clock::time_point expiresAt;
    };

    std::map<std::string, int> inventory;
    std::map<int, Reservation> active;
    std::chrono::seconds ttl;
    int nextReservationId = 1;
    int nextTicketNumber = 1;
    mutable std::mutex mtx;
};
