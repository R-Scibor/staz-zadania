#include "Server.hpp"

#include <chrono>
#include <iostream>

int main() {
    using namespace std::chrono_literals;
    try {
        Server server(5555,
                      {{"normal", 10}, {"ulgowy", 5}},
                      {{"normal", 350}, {"ulgowy", 170}},
                      60s);
        std::cout << "ticketmat server listening on port 5555\n";
        server.run();
    } catch (const std::exception& e) {
        std::cerr << "fatal: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
