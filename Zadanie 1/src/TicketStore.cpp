#include "TicketStore.hpp"

TicketStore::TicketStore(std::map<std::string, int> initialInventory, std::chrono::seconds reservationTtl)
    : inventory(std::move(initialInventory)), ttl(reservationTtl) {}

std::map<std::string, int> TicketStore::getAvailable() const {
    std::lock_guard<std::mutex> lock(mtx);
    return inventory;
}

std::optional<int> TicketStore::reserve(const std::string& type, clock::time_point now) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = inventory.find(type);
    if (it == inventory.end() || it->second <= 0) return std::nullopt;
    it->second--;
    int id = nextReservationId;
    nextReservationId++;
    active[id] = Reservation{type, now + ttl};
    return id;
}

bool TicketStore::cancel(int reservationId) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = active.find(reservationId);
    if (it == active.end()) return false;
    inventory[it->second.type]++;
    active.erase(it);
    return true;
}

std::optional<int> TicketStore::finalize(int reservationId, clock::time_point now) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = active.find(reservationId);
    if (it == active.end()) return std::nullopt;
    if (it->second.expiresAt <= now) {
        inventory[it->second.type]++;
        active.erase(it);
        return std::nullopt;
    }
    active.erase(it);
    return nextTicketNumber++;
}

int TicketStore::releaseExpired(clock::time_point now) {
    std::lock_guard<std::mutex> lock(mtx);
    int count = 0;
    for (auto it = active.begin(); it != active.end(); ) {
        if (it->second.expiresAt <= now) {
            inventory[it->second.type]++;
            it = active.erase(it);
            count++;
        } else {
            it++;
        }
    }
    return count;
}
