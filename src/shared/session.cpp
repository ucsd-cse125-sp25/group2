#include "shared/session.hpp"
#include <stdexcept>

void Session::process_packets(PacketType type, vector<char> payload, uint16_t size) {
    switch (type) {
        case PacketType::INIT:
            {
                std::unique_ptr<Ipacket> packet = deserialize(PacketType::INIT, payload, size);
            }
        case PacketType::STRING:
            {
                std::unique_ptr<Ipacket> packet = deserialize(PacketType::STRING, payload, size);
            }
        default:
            throw std::runtime_error("Unknown packet type");
    }
}