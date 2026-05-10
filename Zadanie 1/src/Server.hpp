#pragma once

#include "TicketStore.hpp"

#include <atomic>
#include <chrono>
#include <cstdint>
#include <map>
#include <string>
#include <thread>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

class Server {
public:
    Server(std::uint16_t port,
           std::map<std::string, int> inventory,
           std::map<std::string, int> ticketPrices,
           std::chrono::seconds reservationTtl);
    ~Server();

    void run();

private:
    void handleConnection(boost::asio::ip::tcp::socket socket);
    void sweeperLoop();

    boost::asio::io_context io;
    boost::asio::ip::tcp::acceptor acceptor;
    TicketStore store;
    std::map<std::string, int> ticketPrices;
    std::thread sweeperThread;
    std::atomic<bool> running{true};
};
