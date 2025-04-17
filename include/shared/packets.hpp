#pragma once
#include <stdint.h>
#include <vector>
#include <cstring>
#include <memory>

using namespace std;

enum class PacketType: uint8_t {
    INIT,
    POSITION,
    STRING
};

struct Ipacket {
    virtual PacketType get_type() const = 0;
    virtual vector<char> serialize() const = 0;
    virtual ~Ipacket() = default;
};

// TESTING
struct StringPacket: public Ipacket {
    string message;

    PacketType get_type() const override {
        return PacketType::STRING;
    }
    vector<char> serialize() const override {
        vector<char> buffer(message.size());
        memcpy(buffer.data(), message.data(), message.size());
        return buffer;
    }
    static StringPacket deserialize(vector<char> payload, uint16_t size) {
        StringPacket packet;
        packet.message = std::string(payload.begin(), payload.begin() + size);
        return packet;
    }
};

struct InitPacket : public Ipacket {
    int client_id;

    PacketType get_type() const override {
        return PacketType::INIT;
    }
    vector<char> serialize() const override {
        vector<char> buffer (sizeof(client_id));
        memcpy(buffer.data(), &client_id, sizeof(client_id));
        return buffer;
    }
    static InitPacket deserialize(const vector<char>& payload, uint16_t size) {
        InitPacket packet;
        memcpy(&packet.client_id, payload.data(), size);
        return packet;
    }
};

struct PositionPacket : public Ipacket {
    float x,y,z;

    PacketType get_type() const override {
        return PacketType::POSITION;
    }
    vector<char> serialize() const override {
        vector<char> buffer (sizeof(x) + sizeof(y) + sizeof(z));
        memcpy(buffer.data(), &x, sizeof(x));
        memcpy(buffer.data() + sizeof(x), &y, sizeof(y));
        memcpy(buffer.data() + sizeof(x) + sizeof(y), &z, sizeof(z));
        return buffer;
    }
    static PositionPacket deserialize(const vector<char>& payload, uint16_t size) {
        PositionPacket packet;
        memcpy(&packet.x, payload.data(), sizeof(float));
        memcpy(&packet.y, payload.data() + sizeof(float), sizeof(float));
        memcpy(&packet.z, payload.data() + 2 * sizeof(float), sizeof(float));
        return packet;
    }
};

inline std::unique_ptr<Ipacket> deserialize(PacketType type, vector<char> payload, uint16_t size) {
    switch (type) {
        case PacketType::STRING:
            return std::make_unique<StringPacket>(StringPacket::deserialize(payload, size));
        case PacketType::INIT:
            return std::make_unique<InitPacket>(InitPacket::deserialize(payload, size));
        case PacketType::POSITION:
            return std::make_unique<PositionPacket>(PositionPacket::deserialize(payload, size));
        default:
            throw std::runtime_error("Unknown packet type");
    }
}