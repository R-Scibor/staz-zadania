#include "Server.hpp"

#include <nlohmann/json.hpp>

#include <boost/asio.hpp>

#include <optional>
#include <string>

using boost::asio::ip::tcp;
using nlohmann::json;

Server::Server(std::uint16_t port,
               std::map<std::string, int> inventory,
               std::map<std::string, int> prices,
               std::chrono::seconds reservationTtl)
    : acceptor(io, tcp::endpoint(tcp::v4(), port)),
      store(std::move(inventory), reservationTtl),
      ticketPrices(std::move(prices)) {
    sweeperThread = std::thread(&Server::sweeperLoop, this);
}

Server::~Server() {
    running = false;
    boost::system::error_code ec;
    acceptor.close(ec);
    if (sweeperThread.joinable()) sweeperThread.join();
}

void Server::run() {
    while (running) {
        tcp::socket socket(io);
        boost::system::error_code ec;
        acceptor.accept(socket, ec);
        if (ec) break;
        std::thread connectionThread(&Server::handleConnection, this, std::move(socket));
        connectionThread.detach();
    }
}

void Server::sweeperLoop() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (!running) break;
        store.releaseExpired(TicketStore::clock::now());
    }
}

void Server::handleConnection(tcp::socket socket) {
    tcp::iostream stream(std::move(socket));
    std::optional<int> currentReservation;

    auto send = [&](const json& resp) {
        stream << resp.dump() << "\n";
        stream.flush();
    };

    auto sendError = [&](const std::string& code, const std::string& detail = "") {
        json resp{{"error", code}};
        if (!detail.empty()) resp["detail"] = detail;
        send(resp);
    };

    std::string line;
    while (std::getline(stream, line)) {
        json req;
        try {
            req = json::parse(line);
        } catch (const std::exception&) {
            sendError("bad_request", "invalid json");
            continue;
        }

        std::string verb;
        try {
            verb = req.at("verb").get<std::string>();
        } catch (const std::exception&) {
            sendError("bad_request", "missing or invalid 'verb'");
            continue;
        }

        try {
            if (verb == "LIST_TICKETS") {
                json arr = json::array();
                auto avail = store.getAvailable();
                for (const auto& [type, price] : ticketPrices) {
                    arr.push_back({{"type", type}, {"price", price}, {"available", avail[type]}});
                }
                send({{"tickets", arr}});

            } else if (verb == "RESERVE") {
                std::string type = req.at("type").get<std::string>();
                if (ticketPrices.find(type) == ticketPrices.end()) {
                    sendError("unknown_type");
                    continue;
                }
                if (currentReservation) {
                    store.cancel(*currentReservation);
                    currentReservation.reset();
                }
                auto id = store.reserve(type, TicketStore::clock::now());
                if (!id) {
                    sendError("sold_out");
                    continue;
                }
                currentReservation = *id;
                send({{"reservationId", *id}});

            } else if (verb == "FINALIZE") {
                int reqId = req.at("reservationId").get<int>();
                std::string name = req.at("name").get<std::string>();
                if (!currentReservation || *currentReservation != reqId) {
                    sendError("bad_request", "reservationId does not match this connection");
                    continue;
                }
                auto num = store.finalize(reqId, name, TicketStore::clock::now());
                currentReservation.reset();
                if (!num) {
                    sendError("expired");
                    continue;
                }
                send({{"ticketNumber", *num}});

            } else if (verb == "CANCEL") {
                if (currentReservation) {
                    store.cancel(*currentReservation);
                    currentReservation.reset();
                }
                send({{"cancelled", true}});

            } else {
                sendError("bad_request", "unknown verb");
            }
        } catch (const std::exception& e) {
            sendError("bad_request", e.what());
        }
    }

    if (currentReservation) {
        store.cancel(*currentReservation);
    }
}
