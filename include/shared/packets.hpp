#pragma once

#include "base_gameobject.hpp"
#include "core.hpp"
#include "globals.hpp"
#include "transform.hpp"
#include "utilities/util_packets.hpp"

#include <cstring>
#include <memory>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <vector>

using namespace std;

enum class PacketType : uint8_t {
  // Sender: Server
  INIT,
  OBJECT,
  GAMESTATE,
  CHARACTERRESPONSE,
  // Sender: Client
  MOVEMENT,
  ROTATION,
  INTERACTION,
  CHARACTERSELECT,
  DISCONNECT
};

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

  ObjectPacket(int id, Transform transform, bool active)
      : objectID(id), transform(transform), active(active) {}
  PacketType getType() const override { return PacketType::OBJECT; }
  vector<char> serialize() const override;
  static ObjectPacket deserialize(const vector<char> &payload);
};

struct GameStatePacket : public IPacket {
  Gamestate state;
  GameStatePacket(Gamestate state) : state(state) {}
  PacketType getType() const override { return PacketType::GAMESTATE; }
  vector<char> serialize() const override;
  static GameStatePacket deserialize(const vector<char> &payload);
};
struct CharacterResponsePacket : public IPacket {
  int chicken;
  int sheep;
  int pig;
  int cow;

  CharacterResponsePacket(int chicken, int sheep, int pig, int cow)
      : chicken(chicken), sheep(sheep), pig(pig), cow(cow) {}
  PacketType getType() const override { return PacketType::CHARACTERRESPONSE; }
  vector<char> serialize() const override;
  static CharacterResponsePacket deserialize(const vector<char> &payload);
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

struct RotationPacket : public IPacket {
  int objectID;
  glm::vec3 rotation;

  RotationPacket(int id, glm::vec3 rotation)
      : objectID(id), rotation(rotation) {}
  PacketType getType() const override { return PacketType::ROTATION; }
  vector<char> serialize() const override;
  static RotationPacket deserialize(const vector<char> &payload);
};

struct InteractionPacket : public IPacket {
  glm::vec3 rayDirection;
  glm::vec3 rayOrigin;

  InteractionPacket(glm::vec3 rayDirection, glm::vec3 rayOrigin)
      : rayDirection(rayDirection), rayOrigin(rayOrigin) {}
  PacketType getType() const override { return PacketType::INTERACTION; }
  vector<char> serialize() const override;
  static InteractionPacket deserialize(const vector<char> &payload);
};

struct CharacterSelectPacket : public IPacket {
  Characters character;
  int clientID;

  CharacterSelectPacket(Characters character, int clientID)
      : character(character), clientID(clientID) {}
  PacketType getType() const override { return PacketType::CHARACTERSELECT; }
  vector<char> serialize() const override;
  static CharacterSelectPacket deserialize(const vector<char> &payload);
};

struct DisconnectPacket : public IPacket {
  int clientID;

  DisconnectPacket(int id) : clientID(id) {}
  PacketType getType() const override { return PacketType::DISCONNECT; }
  vector<char> serialize() const override;
  static DisconnectPacket deserialize(const vector<char> &payload);
};

unique_ptr<IPacket> deserialize(PacketType type, vector<char> &payload);