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

  InitPacket(CLIENT_ID clientID) : id(clientID) {}
  PacketType getType() const override { return PacketType::INIT; }
  vector<char> serialize() const override;
  static InitPacket deserialize(const vector<char> &payload);
};

struct ObjectPacket : public IPacket {
  OBJECT_ID id;
  Transform transform;
  bool active;

  ObjectPacket(OBJECT_ID objectID, Transform t, bool a)
      : id(objectID), transform(t), active(a) {}
  PacketType getType() const override { return PacketType::OBJECT; }
  vector<char> serialize() const override;
  static ObjectPacket deserialize(const vector<char> &payload);
};

struct GameStatePacket : public IPacket {
  Gamestate state;
  GameStatePacket(Gamestate s) : state(s) {}
  PacketType getType() const override { return PacketType::GAMESTATE; }
  vector<char> serialize() const override;
  static GameStatePacket deserialize(const vector<char> &payload);
};

struct CharacterResponsePacket : public IPacket {
  CLIENT_ID characterAssignments[NUM_PLAYERS];

  CharacterResponsePacket(const CLIENT_ID chAssignments[NUM_PLAYERS]) {
    memcpy(characterAssignments, chAssignments,
           NUM_PLAYERS * sizeof(CLIENT_ID));
  }

  PacketType getType() const override { return PacketType::CHARACTERRESPONSE; }
  vector<char> serialize() const override;
  static CharacterResponsePacket deserialize(const vector<char> &payload);
};

struct MovementPacket : public IPacket {
  PLAYER_ID id;
  MovementType movementType;

  MovementPacket(PLAYER_ID playerID, MovementType type)
      : id(playerID), movementType(type) {}
  PacketType getType() const override { return PacketType::MOVEMENT; }
  vector<char> serialize() const override;
  static MovementPacket deserialize(const vector<char> &payload);
};

struct RotationPacket : public IPacket {
  PLAYER_ID id;
  glm::vec3 rotation;

  RotationPacket(PLAYER_ID playerID, glm::vec3 r) : id(playerID), rotation(r) {}
  PacketType getType() const override { return PacketType::ROTATION; }
  vector<char> serialize() const override;
  static RotationPacket deserialize(const vector<char> &payload);
};

struct InteractionPacket : public IPacket {
  PLAYER_ID id;

  InteractionPacket(PLAYER_ID playerID)
      : id(playerID) {}
  PacketType getType() const override { return PacketType::INTERACTION; }
  vector<char> serialize() const override;
  static InteractionPacket deserialize(const vector<char> &payload);
};

struct CharacterSelectPacket : public IPacket {
  PLAYER_ID playerID;
  CLIENT_ID clientID;

  CharacterSelectPacket(PLAYER_ID pID, CLIENT_ID cID)
      : playerID(pID), clientID(cID) {}
  PacketType getType() const override { return PacketType::CHARACTERSELECT; }
  vector<char> serialize() const override;
  static CharacterSelectPacket deserialize(const vector<char> &payload);
};

struct DisconnectPacket : public IPacket {
  CLIENT_ID id;

  DisconnectPacket(CLIENT_ID cID) : id(cID) {}
  PacketType getType() const override { return PacketType::DISCONNECT; }
  vector<char> serialize() const override;
  static DisconnectPacket deserialize(const vector<char> &payload);
};

unique_ptr<IPacket> deserialize(PacketType type, vector<char> &payload);