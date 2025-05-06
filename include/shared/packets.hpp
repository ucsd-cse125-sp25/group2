#pragma once
#include "shared/core.hpp"
#include "shared/gameobject.hpp"
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
  MOVEMENT,
  OBJECT,
  DISCONNECT
};

enum class MovementType: uint8_t { FORWARD, BACKWARD, LEFT, RIGHT };

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

struct MovementPacket : public IPacket {
  int objectID;
  MovementType type;
  MovementPacket(int objectID, MovementType t) : objectID(objectID), type(t) {}
  PacketType get_type() const override { return PacketType::MOVEMENT; }
  vector<char> serialize() const override;
  static MovementPacket deserialize(const vector<char> &payload);
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