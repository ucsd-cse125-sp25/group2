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
  vector<char> buffer(sizeof(int) + sizeof(Transform) + 2 * sizeof(bool));
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
  default:
    throw runtime_error("Unknown packet type");
  }
}