#pragma once

#include <boost/asio/ip/tcp.hpp>

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

class Sale {
public:
    struct TicketInfo {
        std::string type;
        int price;
        int available;
    };

    Sale(const std::string& host, std::uint16_t port);

    std::vector<TicketInfo> listTickets();
    std::optional<int> reserve(const std::string& type);
    std::optional<int> finalize(int reservationId, const std::string& name);
    void cancel(int reservationId);

private:
    boost::asio::ip::tcp::iostream stream;
};
