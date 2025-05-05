#pragma once
#include "shared/core.hpp"
#include "shared/transform.hpp"
#include "shared/utilities/util_packets.hpp"
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

enum class ActionType : uint8_t { FORWARD, BACKWARD, LEFT, RIGHT };

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
  vector<char> serialize() const override;
  static InitPacket deserialize(const vector<char> &payload);
};

struct PositionPacket : public IPacket {
  int object_id;
  Transform transform;

  PositionPacket(int object_id, Transform transform)
      : object_id(object_id), transform(transform) {}
  PacketType get_type() const override { return PacketType::POSITION; }
  vector<char> serialize() const override;
  static PositionPacket deserialize(const vector<char> &payload);
};

struct ActionPacket : public IPacket {
  int objectID;
  ActionType type;
  ActionPacket(int objectID, ActionType t) : objectID(objectID), type(t) {}
  PacketType get_type() const override { return PacketType::ACTION; }
  vector<char> serialize() const override;
  static ActionPacket deserialize(const vector<char> &payload);
};

struct ObjectPacket : public IPacket {
  int id;
  ObjectType type;
  Transform transform;
  bool interactable;
  bool active;

  ObjectPacket(int id, ObjectType type, Transform transform,
               bool interactable = false, bool active = false)
      : id(id), type(type), transform(transform), interactable(interactable),
        active(active) {}
  PacketType get_type() const override { return PacketType::OBJECT; }
  vector<char> serialize() const override;
  static ObjectPacket deserialize(const vector<char> &payload);
};

struct DisconnectPacket : public IPacket {
  int client_id;

  DisconnectPacket(int id) : client_id(id) {}
  PacketType get_type() const override { return PacketType::DISCONNECT; }
  vector<char> serialize() const override;
  static DisconnectPacket deserialize(const vector<char> &payload);
};

std::unique_ptr<IPacket> deserialize(PacketType type, vector<char> &payload);

// Debugging Methods
void printObjectPacket(const ObjectPacket &packet);
void printPositionPacket(const PositionPacket &packet);