#pragma once
#include "shared/core.hpp"
#include "shared/transform.hpp"
#include <cstring>
#include <memory>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <vector>

using namespace std;

enum class PacketType : uint8_t {
  INIT,
  POSITION,
  STRING,
  ACTION,
  OBJECT,
  DISCONNECT
};

enum class ActionType : uint8_t { FORWARD, RIGHT, LEFT, BACK };

enum class ObjectType : uint8_t { CUBE };

struct IPacket {
  virtual PacketType get_type() const = 0;
  virtual vector<char> serialize() const = 0;
  virtual ~IPacket() = default;
};

struct InitPacket : public IPacket {
  int client_id;

  InitPacket(int id) : client_id(id) {}
  PacketType get_type() const override { return PacketType::INIT; }
  vector<char> serialize() const override {
    vector<char> buffer(sizeof(client_id));
    memcpy(buffer.data(), &client_id, sizeof(client_id));
    return buffer;
  }
  static InitPacket deserialize(const vector<char> &payload) {
    int id;
    memcpy(&id, payload.data(), sizeof(int));
    InitPacket packet(id);
    return packet;
  }
};

struct PositionPacket : public IPacket {
  int object_id;
  glm::vec3 position;

  PositionPacket(int object_id, glm::vec3 position)
      : object_id(object_id), position(position) {}
  PacketType get_type() const override { return PacketType::POSITION; }
  vector<char> serialize() const override {
    vector<char> buffer(sizeof(int) + sizeof(glm::vec3));

    unsigned long size = 0;
    memcpy(buffer.data(), &object_id, sizeof(int));
    size += sizeof(int);
    memcpy(buffer.data() + size, &position.x, sizeof(position.x));
    size += sizeof(position.x);
    memcpy(buffer.data() + size, &position.y, sizeof(position.y));
    size += sizeof(position.y);
    memcpy(buffer.data() + size, &position.z, sizeof(position.z));
    return buffer;
  }
  static PositionPacket deserialize(const vector<char> &payload) {
    int object_id;
    glm::vec3 position;

    unsigned long size = 0;

    memcpy(&object_id, payload.data(), sizeof(int));
    size += sizeof(int);
    memcpy(&position.x, payload.data() + size, sizeof(float));
    size += sizeof(float);
    memcpy(&position.y, payload.data() + size, sizeof(float));
    size += sizeof(float);
    memcpy(&position.z, payload.data() + size, sizeof(float));
    PositionPacket packet(object_id, position);

    return packet;
  }
};

struct ActionPacket : public IPacket {
  ActionType type;
  ActionPacket(ActionType t) : type(t) {}
  PacketType get_type() const override { return PacketType::ACTION; }
  vector<char> serialize() const override {
    vector<char> buffer(sizeof(type));
    memcpy(buffer.data(), &type, sizeof(type));
    return buffer;
  }
  static ActionPacket deserialize(const vector<char> &payload) {
    ActionType type;
    memcpy(&type, payload.data(), sizeof(ActionType));
    ActionPacket packet(static_cast<ActionType>(type));
    return packet;
  }
};

struct ObjectPacket : public IPacket {
  int id;
  ObjectType type;
  Transform transform;
  bool interactable;
  bool active;

  ObjectPacket(int id, ObjectType type, Transform transform,
               bool interactable = false, bool active = false)
      : id(id), type(type), transform(transform), interactable(false),
        active(active) {}
  PacketType get_type() const override { return PacketType::OBJECT; }

  vector<char> serialize() const override;

  static ObjectPacket deserialize(const vector<char> &payload);
};

struct DisconnectPacket : public IPacket {
  int client_id;

  DisconnectPacket(int id) : client_id(id) {}
  PacketType get_type() const override { return PacketType::DISCONNECT; }
  vector<char> serialize() const override {
    vector<char> buffer(sizeof(client_id));
    memcpy(buffer.data(), &client_id, sizeof(client_id));
    return buffer;
  }
  static DisconnectPacket deserialize(const vector<char> &payload) {
    int id;
    memcpy(&id, payload.data(), sizeof(int));
    DisconnectPacket packet(id);
    return packet;
  }
};

inline std::unique_ptr<IPacket> deserialize(PacketType type,
                                            vector<char> payload) {
  switch (type) {
  case PacketType::INIT:
    return std::make_unique<InitPacket>(InitPacket::deserialize(payload));
  case PacketType::POSITION:
    return std::make_unique<PositionPacket>(
        PositionPacket::deserialize(payload));
  case PacketType::ACTION:
    return std::make_unique<ActionPacket>(ActionPacket::deserialize(payload));
  case PacketType::OBJECT:
    return std::make_unique<ObjectPacket>(ObjectPacket::deserialize(payload));
  case PacketType::DISCONNECT:
    return std::make_unique<DisconnectPacket>(
        DisconnectPacket::deserialize(payload));
  default:
    throw runtime_error("Unknown packet type");
  }
}

// TESTING
// struct StringPacket : public IPacket
// {
//     string message;

//     StringPacket(string m) : message(m) {}
//     PacketType get_type() const override
//     {
//         return PacketType::STRING;
//     }
//     vector<char> serialize() const override
//     {
//         vector<char> buffer(message.size());
//         memcpy(buffer.data(), message.data(), message.size());
//         return buffer;
//     }
//     static StringPacket deserialize(vector<char> payload, uint16_t size)
//     {
//         StringPacket packet(string(payload.begin(), payload.begin() + size));
//         return packet;
//     }
// };