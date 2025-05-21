#include "packets.hpp"

vector<char> InitPacket::serialize() const {
  vector<char> buffer(sizeof(int));
  memcpy(buffer.data(), &id, sizeof(int));
  return buffer;
}

InitPacket InitPacket::deserialize(const vector<char> &payload) {
  CLIENT_ID id;

  memcpy(&id, payload.data(), sizeof(int));
  InitPacket packet(id);
  return packet;
}

vector<char> ObjectPacket::serialize() const {
  vector<char> buffer(sizeof(int) + sizeof(Transform) + sizeof(bool));
  unsigned long size = 0;
  memcpy(buffer.data(), &id, sizeof(int));
  size += sizeof(int);
  serializeTransform(buffer.data(), transform, size);
  size += sizeof(bool);
  memcpy(buffer.data() + size, &active, sizeof(bool));
  return buffer;
}

ObjectPacket ObjectPacket::deserialize(const vector<char> &payload) {
  OBJECT_ID id;
  Transform transform;
  bool active;

  unsigned long size = 0;
  memcpy(&id, payload.data(), sizeof(OBJECT_ID));
  size += sizeof(OBJECT_ID);
  deserializeTransform(payload, transform, size);
  memcpy(&active, payload.data() + size, sizeof(bool));
  ObjectPacket packet(id, transform, active);
  packet.active = active;
  return packet;
}

vector<char> GameStatePacket::serialize() const {
  vector<char> buffer(sizeof(uint8_t));
  memcpy(buffer.data(), &state, sizeof(uint8_t));
  return buffer;
}

GameStatePacket GameStatePacket::deserialize(const vector<char> &payload) {
  Gamestate state;

  memcpy(&state, payload.data(), sizeof(uint8_t));
  GameStatePacket packet(state);
  return packet;
}

vector<char> CharacterResponsePacket::serialize() const {
  vector<char> buffer(sizeof(int) * 4);

  unsigned long size = 0;
  memcpy(buffer.data(), &chicken, sizeof(int));
  size += sizeof(int);
  memcpy(buffer.data() + size, &sheep, sizeof(int));
  size += sizeof(int);
  memcpy(buffer.data() + size, &pig, sizeof(int));
  size += sizeof(int);
  memcpy(buffer.data() + size, &cow, sizeof(int));
  return buffer;
}

CharacterResponsePacket
CharacterResponsePacket::deserialize(const vector<char> &payload) {
  PLAYER_ID chicken;
  PLAYER_ID sheep;
  PLAYER_ID pig;
  PLAYER_ID cow;

  unsigned long size = 0;
  memcpy(&chicken, payload.data(), sizeof(PLAYER_ID));
  size += sizeof(PLAYER_ID);
  memcpy(&sheep, payload.data() + size, sizeof(PLAYER_ID));
  size += sizeof(PLAYER_ID);
  memcpy(&pig, payload.data() + size, sizeof(PLAYER_ID));
  size += sizeof(int);
  memcpy(&cow, payload.data() + size, sizeof(int));
  CharacterResponsePacket packet(chicken, sheep, pig, cow);
  return packet;
}

vector<char> MovementPacket::serialize() const {
  vector<char> buffer(sizeof(OBJECT_ID) + sizeof(MovementType) +
                      sizeof(glm::vec3));
  unsigned long size = 0;
  memcpy(buffer.data(), &id, sizeof(OBJECT_ID));
  size += sizeof(OBJECT_ID);
  memcpy(buffer.data() + size, &movementType, sizeof(MovementType));
  size += sizeof(MovementType);
  serializeVector(buffer.data(), cameraFront, size);
  return buffer;
}

MovementPacket MovementPacket::deserialize(const vector<char> &payload) {
  OBJECT_ID id;
  MovementType movementType;
  glm::vec3 cameraFront;

  unsigned long size = 0;
  memcpy(&id, payload.data(), sizeof(OBJECT_ID));
  size += sizeof(OBJECT_ID);
  memcpy(&movementType, payload.data() + size, sizeof(MovementType));
  size += sizeof(MovementType);
  cameraFront = deserializeVector(payload, size);
  MovementPacket packet(id, movementType, cameraFront);
  return packet;
}

vector<char> RotationPacket::serialize() const {
  vector<char> buffer(sizeof(OBJECT_ID) + sizeof(glm::vec3));
  unsigned long size = 0;
  memcpy(buffer.data(), &id, sizeof(OBJECT_ID));
  size += sizeof(OBJECT_ID);
  serializeVector(buffer.data(), rotation, size);
  return buffer;
}

RotationPacket RotationPacket::deserialize(const vector<char> &payload) {
  OBJECT_ID id;
  glm::vec3 rotation;

  unsigned long size = 0;
  memcpy(&id, payload.data(), sizeof(OBJECT_ID));
  size += sizeof(OBJECT_ID);
  rotation = deserializeVector(payload, size);
  RotationPacket packet(id, rotation);
  return packet;
}

vector<char> InteractionPacket::serialize() const {
  vector<char> buffer(sizeof(PLAYER_ID) + sizeof(glm::vec3) * 2);
  ;
  unsigned long size = 0;

  memcpy(buffer.data(), &character, sizeof(PLAYER_ID));
  size += sizeof(PLAYER_ID);
  serializeVector(buffer.data(), rayDirection, size);
  serializeVector(buffer.data(), rayOrigin, size);
  return buffer;
}

InteractionPacket InteractionPacket::deserialize(const vector<char> &payload) {
  PLAYER_ID character;
  glm::vec3 rayDirection;
  glm::vec3 rayOrigin;

  unsigned long size = 0;
  memcpy(&character, payload.data(), sizeof(PLAYER_ID));
  size += sizeof(PLAYER_ID);
  rayDirection = deserializeVector(payload, size);
  rayOrigin = deserializeVector(payload, size);
  InteractionPacket packet(character, rayDirection, rayOrigin);
  return packet;
}

vector<char> CharacterResponsePacket::serialize() const {
  vector<char> buffer(sizeof(int) * NUM_PLAYERS);

  unsigned long size = 0;
  memcpy(buffer.data(), &chicken, sizeof(PLAYER_ID));
  size += sizeof(PLAYER_ID);
  memcpy(buffer.data() + size, &sheep, sizeof(PLAYER_ID));
  size += sizeof(PLAYER_ID);
  memcpy(buffer.data() + size, &pig, sizeof(PLAYER_ID));
  size += sizeof(PLAYER_ID);
  memcpy(buffer.data() + size, &cow, sizeof(PLAYER_ID));
  return buffer;
}

CharacterResponsePacket
CharacterResponsePacket::deserialize(const vector<char> &payload) {
  PLAYER_ID chicken;
  PLAYER_ID sheep;
  PLAYER_ID pig;
  PLAYER_ID cow;

  unsigned long size = 0;
  memcpy(&chicken, payload.data(), sizeof(PLAYER_ID));
  size += sizeof(PLAYER_ID);
  memcpy(&sheep, payload.data() + size, sizeof(PLAYER_ID));
  size += sizeof(PLAYER_ID);
  memcpy(&pig, payload.data() + size, sizeof(PLAYER_ID));
  size += sizeof(PLAYER_ID);
  memcpy(&cow, payload.data() + size, sizeof(PLAYER_ID));
  CharacterResponsePacket packet(chicken, sheep, pig, cow);
  return packet;
}

vector<char> DisconnectPacket::serialize() const {
  vector<char> buffer(sizeof(int));
  memcpy(buffer.data(), &id, sizeof(int));
  return buffer;
}

DisconnectPacket DisconnectPacket::deserialize(const vector<char> &payload) {
  CLIENT_ID id;

  memcpy(&id, payload.data(), sizeof(CLIENT_ID));
  DisconnectPacket packet(id);
  return packet;
}

unique_ptr<IPacket> deserialize(PacketType type, vector<char> &payload) {
  switch (type) {
  case PacketType::INIT:
    return make_unique<InitPacket>(InitPacket::deserialize(payload));
  case PacketType::OBJECT:
    return make_unique<ObjectPacket>(ObjectPacket::deserialize(payload));
  case PacketType::GAMESTATE:
    return make_unique<GameStatePacket>(GameStatePacket::deserialize(payload));
  case PacketType::CHARACTERRESPONSE:
    return make_unique<CharacterResponsePacket>(
        CharacterResponsePacket::deserialize(payload));
  case PacketType::MOVEMENT:
    return make_unique<MovementPacket>(MovementPacket::deserialize(payload));
  case PacketType::ROTATION:
    return make_unique<RotationPacket>(RotationPacket::deserialize(payload));
  case PacketType::INTERACTION:
    return make_unique<InteractionPacket>(
        InteractionPacket::deserialize(payload));
  case PacketType::CHARACTERSELECT:
    return make_unique<CharacterSelectPacket>(
        CharacterSelectPacket::deserialize(payload));
  case PacketType::DISCONNECT:
    return make_unique<DisconnectPacket>(
        DisconnectPacket::deserialize(payload));
  default:
    throw runtime_error("Unknown packet type");
  }
}