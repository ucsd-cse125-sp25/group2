#pragma once
#include <stdint.h>
#include <vector>
#include <cstring>
#include <string>
#include <memory>
#include <stdexcept>

using namespace std;

enum class PacketType : uint8_t
{
    INIT,
    POSITION,
    STRING,
    ACTION,
    DISCONNECT
};

enum class ActionType : uint8_t
{
    FORWARD,
    RIGHT,
    LEFT,
    BACK
};

struct IPacket
{
    virtual PacketType get_type() const = 0;
    virtual vector<char> serialize() const = 0;
    virtual ~IPacket() = default;
};

// TESTING
struct StringPacket : public IPacket
{
    string message;

    StringPacket(string m) : message(m) {}
    PacketType get_type() const override
    {
        return PacketType::STRING;
    }
    vector<char> serialize() const override
    {
        vector<char> buffer(message.size());
        memcpy(buffer.data(), message.data(), message.size());
        return buffer;
    }
    static StringPacket deserialize(vector<char> payload, uint16_t size)
    {
        StringPacket packet(string(payload.begin(), payload.begin() + size));
        return packet;
    }
};

struct InitPacket : public IPacket
{
    int client_id;

    InitPacket(int id) : client_id(id) {}
    PacketType get_type() const override
    {
        return PacketType::INIT;
    }
    vector<char> serialize() const override
    {
        vector<char> buffer(sizeof(client_id));
        memcpy(buffer.data(), &client_id, sizeof(client_id));
        return buffer;
    }
    static InitPacket deserialize(const vector<char> &payload, uint16_t size)
    {
        int id;
        memcpy(&id, payload.data(), size);
        InitPacket packet(id);
        return packet;
    }
};

struct PositionPacket : public IPacket
{
    float x, y, z;

    PositionPacket(float x, float y, float z) : x(x), y(y), z(z) {}
    PacketType get_type() const override
    {
        return PacketType::POSITION;
    }
    vector<char> serialize() const override
    {
        vector<char> buffer(sizeof(x) + sizeof(y) + sizeof(z));
        memcpy(buffer.data(), &x, sizeof(x));
        memcpy(buffer.data() + sizeof(x), &y, sizeof(y));
        memcpy(buffer.data() + sizeof(x) + sizeof(y), &z, sizeof(z));
        return buffer;
    }
    static PositionPacket deserialize(const vector<char> &payload, uint16_t size)
    {
        float x, y, z;

        memcpy(&x, payload.data(), sizeof(float));
        memcpy(&y, payload.data() + sizeof(float), sizeof(float));
        memcpy(&z, payload.data() + 2 * sizeof(float), sizeof(float));
        PositionPacket packet(x, y, z);

        return packet;
    }
};

struct ActionPacket : public IPacket
{
    ActionType type;
    ActionPacket(ActionType t) : type(t) {}
    PacketType get_type() const override
    {
        return PacketType::ACTION;
    }
    vector<char> serialize() const override
    {
        vector<char> buffer(sizeof(type));
        memcpy(buffer.data(), &type, sizeof(type));
        return buffer;
    }
    static ActionPacket deserialize(const vector<char> &payload, uint16_t size)
    {
        ActionType type;
        memcpy(&type, payload.data(), sizeof(ActionType));
        ActionPacket packet(static_cast<ActionType>(type));
        return packet;
    }
};

struct DisconnectPacket: public IPacket
{
    int client_id;

    DisconnectPacket(int id) : client_id(id) {}
    PacketType get_type() const override
    {
        return PacketType::DISCONNECT;
    }
    vector<char> serialize() const override
    {
        vector<char> buffer(sizeof(client_id));
        memcpy(buffer.data(), &client_id, sizeof(client_id));
        return buffer;
    }
    static DisconnectPacket deserialize(const vector<char> &payload, uint16_t size)
    {
        int id;
        memcpy(&id, payload.data(), size);
        DisconnectPacket packet(id);
        return packet;
    }
};

inline std::unique_ptr<IPacket> deserialize(PacketType type, vector<char> payload, uint16_t size)
{
    switch (type)
    {
    case PacketType::STRING:
        return std::make_unique<StringPacket>(StringPacket::deserialize(payload, size));
    case PacketType::INIT:
        return std::make_unique<InitPacket>(InitPacket::deserialize(payload, size));
    case PacketType::POSITION:
        return std::make_unique<PositionPacket>(PositionPacket::deserialize(payload, size));
    case PacketType::ACTION:
        return std::make_unique<ActionPacket>(ActionPacket::deserialize(payload, size));
    case PacketType::DISCONNECT:
        return std::make_unique<DisconnectPacket>(DisconnectPacket::deserialize(payload, size));
    default:
        throw runtime_error("Unknown packet type");
    }
}