#pragma once
#include <stdint.h>
#include <vector>
#include <cstring>


using namespace std;

enum class PacketType: uint8_t {
    INIT,
    TRANSFORM,
    INPUT, 
    STRING,
};
struct Ipacket {
    virtual PacketType get_type() const = 0;
    virtual vector<char> serialize() const = 0;
    virtual ~Ipacket() = default;
};

// for testing
struct string_packet: public Ipacket {
    string message;

    PacketType get_type() const override {
        return PacketType::STRING;
    }
    vector<char> serialize() const override {
        vector<char> buffer(message.size());

        memcpy(buffer.data(), message.data(), message.size());

        return buffer;
    }
};

struct init_packet : public Ipacket {
    int client_id;

    PacketType get_type() const override {
        return PacketType::INIT;
    }
    vector<char> serialize() const override {
        vector<char> buffer (sizeof(client_id));
        memcpy(buffer.data(), &client_id, sizeof(client_id));
        return buffer;
    }
};

struct input_packet : public Ipacket {
    PacketType get_type() const override {
        return PacketType::INPUT;
    }
};
struct transform_packet : public Ipacket {
    float x,y,z;
    float rotation;
    float scale;
    PacketType get_type() const override {
        return PacketType::TRANSFORM;
    }
    vector<char> serialize() const override {
        vector<char> buffer (sizeof(x) + sizeof(y) + sizeof(z) + sizeof(rotation) + sizeof(scale));
        memcpy(buffer.data(), &x, sizeof(x));
        memcpy(buffer.data() + sizeof(x), &y, sizeof(y));
        memcpy(buffer.data() + sizeof(x) + sizeof(y), &z, sizeof(z));
        memcpy(buffer.data() + sizeof(x) + sizeof(y) + sizeof(z), &rotation, sizeof(rotation));
        memcpy(buffer.data() + sizeof(x) + sizeof(y) + sizeof(z) + sizeof(rotation), &scale, sizeof(scale));
        return buffer;
    }
};