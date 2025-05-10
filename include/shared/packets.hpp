#pragma once

#include "base_gameobject.hpp"
#include "core.hpp"
#include "transform.hpp"
#include "globals.hpp"
#include "utilities/util_packets.hpp"

#include <cstring>
#include <memory>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <vector>

using namespace std;

enum class PacketType : uint8_t { INIT, OBJECT, MOVEMENT, INTERACTION, DISCONNECT };

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
  Transform transform;
  bool active;

  ObjectPacket(int id, Transform transform,
              bool active)
      : objectID(id), transform(transform),
        active(active) {}
  PacketType getType() const override { return PacketType::OBJECT; }
  vector<char> serialize() const override;
  static ObjectPacket deserialize(const vector<char> &payload);
};

struct MovementPacket : public IPacket {
  int objectID;
  MovementType movementType;
  glm::vec3 cameraFront;

  MovementPacket(int id, MovementType type, glm::vec3 cameraFront)
      : objectID(id), movementType(type), cameraFront(cameraFront) {}
  PacketType getType() const override { return PacketType::MOVEMENT; }
  vector<char> serialize() const override;
  static MovementPacket deserialize(const vector<char> &payload);
};

struct InteractionPacket : public IPacket {
  int objectID;

  InteractionPacket(int id) : objectID(id) {}
  PacketType getType() const override { return PacketType::INTERACTION; }
  vector<char> serialize() const override;
  static InteractionPacket deserialize(const vector<char> &payload);
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