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
  CLIENT_ID id;

  InitPacket(CLIENT_ID id) : id(id) {}
  PacketType getType() const override { return PacketType::INIT; }
  vector<char> serialize() const override;
  static InitPacket deserialize(const vector<char> &payload);
};

struct ObjectPacket : public IPacket {
  OBJECT_ID id;
  Transform transform;
  bool active;

  ObjectPacket(OBJECT_ID id, Transform transform, bool active)
      : id(id), transform(transform), active(active) {}
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
  PLAYER_ID chicken;
  PLAYER_ID sheep;
  PLAYER_ID pig;
  PLAYER_ID cow;

  CharacterResponsePacket(PLAYER_ID chicken, PLAYER_ID sheep, PLAYER_ID pig,
                          PLAYER_ID cow)
      : chicken(chicken), sheep(sheep), pig(pig), cow(cow) {}
  PacketType getType() const override { return PacketType::CHARACTERRESPONSE; }
  vector<char> serialize() const override;
  static CharacterResponsePacket deserialize(const vector<char> &payload);
};

struct MovementPacket : public IPacket {
  OBJECT_ID id;
  MovementType movementType;
  glm::vec3 cameraFront;

  MovementPacket(OBJECT_ID id, MovementType type, glm::vec3 cameraFront)
      : id(id), movementType(type), cameraFront(cameraFront) {}
  PacketType getType() const override { return PacketType::MOVEMENT; }
  vector<char> serialize() const override;
  static MovementPacket deserialize(const vector<char> &payload);
};

struct RotationPacket : public IPacket {
  OBJECT_ID id;
  glm::vec3 rotation;

  RotationPacket(OBJECT_ID id, glm::vec3 rotation)
      : id(id), rotation(rotation) {}
  PacketType getType() const override { return PacketType::ROTATION; }
  vector<char> serialize() const override;
  static RotationPacket deserialize(const vector<char> &payload);
};

struct InteractionPacket : public IPacket {
  PLAYER_ID character;
  glm::vec3 rayDirection;
  glm::vec3 rayOrigin;

  InteractionPacket(PLAYER_ID character, glm::vec3 rayDirection,
                    glm::vec3 rayOrigin)
      : character(character), rayDirection(rayDirection), rayOrigin(rayOrigin) {
  }
  PacketType getType() const override { return PacketType::INTERACTION; }
  vector<char> serialize() const override;
  static InteractionPacket deserialize(const vector<char> &payload);
};

struct CharacterSelectPacket : public IPacket {
  PLAYER_ID character;
  CLIENT_ID id;

  CharacterSelectPacket(PLAYER_ID character, CLIENT_ID id)
      : character(character), id(id) {}
  PacketType getType() const override { return PacketType::CHARACTERSELECT; }
  vector<char> serialize() const override;
  static CharacterSelectPacket deserialize(const vector<char> &payload);
};

struct DisconnectPacket : public IPacket {
  CLIENT_ID id;

  DisconnectPacket(CLIENT_ID id) : id(id) {}
  PacketType getType() const override { return PacketType::DISCONNECT; }
  vector<char> serialize() const override;
  static DisconnectPacket deserialize(const vector<char> &payload);
};

unique_ptr<IPacket> deserialize(PacketType type, vector<char> &payload);