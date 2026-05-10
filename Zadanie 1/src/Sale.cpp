#include "Sale.hpp"

#include <nlohmann/json.hpp>

#include <stdexcept>

using nlohmann::json;

Sale::Sale(const std::string& host, std::uint16_t port)
    : stream(host, std::to_string(port)) {
    if (!stream) throw std::runtime_error("connect failed");
}

static json sendAndReceive(boost::asio::ip::tcp::iostream& stream, const json& req) {
    stream << req.dump() << "\n";
    stream.flush();
    std::string line;
    if (!std::getline(stream, line)) throw std::runtime_error("connection closed");
    return json::parse(line);
}

std::vector<Sale::TicketInfo> Sale::listTickets() {
    json resp = sendAndReceive(stream, {{"verb", "LIST_TICKETS"}});
    std::vector<TicketInfo> out;
    for (const auto& t : resp.at("tickets")) {
        out.push_back({t.at("type").get<std::string>(),
                       t.at("price").get<int>(),
                       t.at("available").get<int>()});
    }
    return out;
}

std::optional<int> Sale::reserve(const std::string& type) {
    json resp = sendAndReceive(stream, {{"verb", "RESERVE"}, {"type", type}});
    if (resp.contains("error")) return std::nullopt;
    return resp.at("reservationId").get<int>();
}

std::optional<int> Sale::finalize(int reservationId, const std::string& name) {
    json resp = sendAndReceive(stream, {{"verb", "FINALIZE"},
                                        {"reservationId", reservationId},
                                        {"name", name}});
    if (resp.contains("error")) return std::nullopt;
    return resp.at("ticketNumber").get<int>();
}

void Sale::cancel(int reservationId) {
    sendAndReceive(stream, {{"verb", "CANCEL"}, {"reservationId", reservationId}});
}
