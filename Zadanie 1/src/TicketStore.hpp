#pragma once

#include <chrono>
#include <map>
#include <mutex>
#include <optional>
#include <string>

class TicketStore {
public:
    using clock = std::chrono::steady_clock;

    struct IssuedTicket {
        int ticketNumber;
        std::string type;
        std::string holderName;
        clock::time_point issuedAt;
    };

    TicketStore(std::map<std::string, int> initialInventory, std::chrono::seconds reservationTtl);

    std::map<std::string, int> getAvailable() const;

    std::optional<int> reserve(const std::string& type, clock::time_point now);

    bool cancel(int reservationId);

    std::optional<int> finalize(int reservationId, const std::string& name, clock::time_point now);

    int releaseExpired(clock::time_point now);

    std::optional<IssuedTicket> getIssued(int ticketNumber) const;

private:
    struct Reservation {
        std::string type;
        clock::time_point expiresAt;
    };

    std::map<std::string, int> inventory;
    std::map<int, Reservation> active;
    std::map<int, IssuedTicket> issued;
    std::chrono::seconds ttl;
    int nextReservationId = 1;
    int nextTicketNumber = 1;
    mutable std::mutex mtx;
};
