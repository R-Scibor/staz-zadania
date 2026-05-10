#include "CashRegister.hpp"
#include "Sale.hpp"

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

static std::string formatPLN(int grosze) {
    int zloty = grosze / 100;
    int gr = grosze % 100;
    std::ostringstream os;
    os << zloty << ".";
    if (gr < 10) os << "0";
    os << gr << " PLN";
    return os.str();
}

static std::vector<int> splitInts(const std::string& line) {
    std::istringstream is(line);
    std::vector<int> out;
    int n;
    while (is >> n) out.push_back(n);
    return out;
}

static std::string formatChangeMap(const std::map<int, int>& m) {
    std::ostringstream os;
    bool first = true;
    for (const auto& [denom, count] : m) {
        if (!first) os << " ";
        os << count << "x" << denom;
        first = false;
    }
    return os.str();
}

int main() {
    CashRegister drawer({{200, 2}, {100, 3}, {50, 3}, {20, 2}, {10, 2}});

    int choice = -1;
    while (choice != 0) {
        try {
            Sale sale("127.0.0.1", 5555);
            auto tickets = sale.listTickets();

            std::cout << std::endl << "=== TICKETMAT ===" << std::endl;
            for (int i = 0; i < (int)tickets.size(); i++) {
                std::cout << (i + 1) << ") " << tickets[i].type
                          << "  " << formatPLN(tickets[i].price)
                          << "  (" << tickets[i].available << " left)" << std::endl;
            }
            std::cout << "0) quit" << std::endl << "Choice: ";

            std::cin >> choice;
            std::cin.ignore();
            if (choice == 0) break;

            int ticketIndex = choice - 1;
            if (ticketIndex < 0 || ticketIndex >= (int)tickets.size()) {
                std::cout << "Invalid choice." << std::endl;
                continue;
            }
            const auto& selected = tickets[ticketIndex];

            auto resId = sale.reserve(selected.type);
            if (!resId) {
                std::cout << "Sold out." << std::endl;
                continue;
            }

            std::cout << "Holder name: ";
            std::string name;
            std::getline(std::cin, name);

            std::cout << "Price: " << formatPLN(selected.price)
                      << ". Insert coins (e.g. \"200 100 50\"): ";
            std::string coinsLine;
            std::getline(std::cin, coinsLine);
            auto inserted = splitInts(coinsLine);

            auto change = drawer.acceptPayment(inserted, selected.price);
            if (!change) {
                std::cout << "Payment refused." << std::endl;
                sale.cancel(*resId);
                continue;
            }

            auto ticketNum = sale.finalize(*resId, name);
            if (!ticketNum) {
                std::cout << "Reservation expired, refunding." << std::endl;
                drawer.refund(inserted, *change);
                continue;
            }

            if (!change->empty()) {
                std::cout << "Change: " << formatChangeMap(*change) << std::endl;
            }
            std::cout << "Ticket #" << *ticketNum << " for " << name << std::endl;

        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
    return 0;
}
