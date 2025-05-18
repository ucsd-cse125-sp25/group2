#include "packets.hpp"

vector<char> InitPacket::serialize() const {
  vector<char> buffer(sizeof(int));
  memcpy(buffer.data(), &clientID, sizeof(int));
  return buffer;
}

InitPacket InitPacket::deserialize(const vector<char> &payload) {
  int clientID;

  memcpy(&clientID, payload.data(), sizeof(int));
  InitPacket packet(clientID);
  return packet;
}

vector<char> ObjectPacket::serialize() const {
  vector<char> buffer(sizeof(int) + sizeof(Transform) + sizeof(bool));
  unsigned long size = 0;
  memcpy(buffer.data(), &objectID, sizeof(int));
  size += sizeof(int);
  serializeTransform(buffer.data(), transform, size);
  size += sizeof(bool);
  memcpy(buffer.data() + size, &active, sizeof(bool));
  return buffer;
}

ObjectPacket ObjectPacket::deserialize(const vector<char> &payload) {
  int objectID;
  Transform transform;
  bool active;

  unsigned long size = 0;
  memcpy(&objectID, payload.data(), sizeof(int));
  size += sizeof(int);
  deserializeTransform(payload, transform, size);
  memcpy(&active, payload.data() + size, sizeof(bool));
  ObjectPacket packet(objectID, transform, active);
  packet.active = active;
  return packet;
}

vector<char> MovementPacket::serialize() const {
  vector<char> buffer(sizeof(int) + sizeof(MovementType) + sizeof(glm::vec3));
  unsigned long size = 0;
  memcpy(buffer.data(), &objectID, sizeof(int));
  size += sizeof(int);
  memcpy(buffer.data() + size, &movementType, sizeof(MovementType));
  size += sizeof(MovementType);
  serializeVector(buffer.data(), cameraFront, size);
  return buffer;
}

MovementPacket MovementPacket::deserialize(const vector<char> &payload) {
  int objectID;
  MovementType movementType;
  glm::vec3 cameraFront;

  unsigned long size = 0;
  memcpy(&objectID, payload.data(), sizeof(int));
  size += sizeof(int);
  memcpy(&movementType, payload.data() + size, sizeof(MovementType));
  size += sizeof(MovementType);
  cameraFront = deserializeVector(payload, cameraFront, size);
  MovementPacket packet(objectID, movementType, cameraFront);
  return packet;
}

vector<char> InteractionPacket::serialize() const {
  vector<char> buffer(sizeof(int));
  memcpy(buffer.data(), &objectID, sizeof(int));
  return buffer;
}

InteractionPacket InteractionPacket::deserialize(const vector<char> &payload) {
  int objectID;

  memcpy(&objectID, payload.data(), sizeof(int));
  InteractionPacket packet(objectID);
  return packet;
}

vector<char> DisconnectPacket::serialize() const {
  vector<char> buffer(sizeof(int));
  memcpy(buffer.data(), &clientID, sizeof(int));
  return buffer;
}

DisconnectPacket DisconnectPacket::deserialize(const vector<char> &payload) {
  int clientID;

  memcpy(&clientID, payload.data(), sizeof(int));
  DisconnectPacket packet(clientID);
  return packet;
}

vector<char> CharacterSelectPacket::serialize() const {
  vector<char> buffer(sizeof(uint8_t) + sizeof(int));

  unsigned long size = 0;
  memcpy(buffer.data(), &character, sizeof(uint8_t));
  size += sizeof(uint8_t);
  memcpy(buffer.data() + size, &clientID, sizeof(int));
  return buffer;
}

CharacterSelectPacket CharacterSelectPacket::deserialize(const vector<char> &payload) {
  Characters character;
  int clientID;

  unsigned long size = 0;
  memcpy(&character, payload.data(), sizeof(uint8_t));
  size += sizeof(uint8_t);
  memcpy(&clientID, payload.data() + size, sizeof(int));
  CharacterSelectPacket packet(character, clientID);
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


CharacterResponsePacket CharacterResponsePacket::deserialize(const vector<char> &payload) {
  int chicken;
  int sheep;
  int pig;
  int cow;

  unsigned long size = 0;
  memcpy(&chicken, payload.data(), sizeof(int));
  size += sizeof(int);
  memcpy(&sheep, payload.data() + size, sizeof(int));
  size += sizeof(int);
  memcpy(&pig, payload.data() + size, sizeof(int));
  size += sizeof(int);
  memcpy(&cow, payload.data() + size, sizeof(int));
  CharacterResponsePacket packet(chicken, sheep, pig, cow);
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

unique_ptr<IPacket> deserialize(PacketType type, vector<char> &payload) {
  switch (type) {
  case PacketType::INIT:
    return make_unique<InitPacket>(InitPacket::deserialize(payload));
  case PacketType::OBJECT:
    return make_unique<ObjectPacket>(ObjectPacket::deserialize(payload));
  case PacketType::MOVEMENT:
    return make_unique<MovementPacket>(MovementPacket::deserialize(payload));
  case PacketType::INTERACTION:
    return make_unique<InteractionPacket>(
        InteractionPacket::deserialize(payload));
  case PacketType::DISCONNECT:
    return make_unique<DisconnectPacket>(
        DisconnectPacket::deserialize(payload));
  case PacketType::CHARACTERSELECT:
      return make_unique<CharacterSelectPacket>(CharacterSelectPacket::deserialize(payload));
  case PacketType::CHARACTERRESPONSE:
      return make_unique<CharacterResponsePacket>(CharacterResponsePacket::deserialize(payload)); 
  case PacketType::GAMESTATE:
      return make_unique<GameStatePacket>(GameStatePacket::deserialize(payload)); 
  default:
    throw runtime_error("Unknown packet type");
  }
}