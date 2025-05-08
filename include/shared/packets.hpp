#pragma once

#include "shared/core.hpp"
#include "shared/gameobject.hpp"
#include "shared/transform.hpp"
#include "shared/utilities/util_packets.hpp"
#include "shared/game_enums.hpp"

#include <cstring>
#include <memory>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <vector>

using namespace std;

enum class PacketType : uint8_t { INIT, OBJECT, MOVEMENT, DISCONNECT };
struct IPacket {
  virtual PacketType getType() const = 0;
  virtual vector<char> serialize() const = 0;
  virtual ~IPacket() = default;
};

struct InitPacket : public IPacket {
  int clientID;

  InitPacket(int id) : clientID(id) {}
  PacketType getType() const override { return PacketType::INIT; }
  vector<char> serialize() const override;
  static InitPacket deserialize(const vector<char> &payload);
};

struct ObjectPacket : public IPacket {
  int objectID;
  ObjectType objectType;
  Transform transform;
  bool interactable;
  bool active;

  ObjectPacket(int id, ObjectType type, Transform transform,
               bool interactable = false, bool active = false)
      : objectID(id), objectType(type), transform(transform),
        interactable(interactable), active(active) {}
  PacketType getType() const override { return PacketType::OBJECT; }
  vector<char> serialize() const override;
  static ObjectPacket deserialize(const vector<char> &payload);
};

struct MovementPacket : public IPacket {
  int objectID;
  MovementType movementType;

  MovementPacket(int id, MovementType type)
      : objectID(id), movementType(type) {}
  PacketType getType() const override { return PacketType::MOVEMENT; }
  vector<char> serialize() const override;
  static MovementPacket deserialize(const vector<char> &payload);
};

struct DisconnectPacket : public IPacket {
  int clientID;

  DisconnectPacket(int id) : clientID(id) {}
  PacketType getType() const override { return PacketType::DISCONNECT; }
  vector<char> serialize() const override;
  static DisconnectPacket deserialize(const vector<char> &payload);
};

unique_ptr<IPacket> deserialize(PacketType type, vector<char> &payload);

// Debugging Methods
void printObjectPacket(const ObjectPacket &packet);