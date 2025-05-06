#include "shared/packets.hpp"

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
  vector<char> buffer(sizeof(int) + sizeof(ObjectType) + sizeof(Transform)  + 2 * sizeof(bool));
  unsigned long size = 0;
  memcpy(buffer.data(), &objectID, sizeof(int));
  size += sizeof(int);
  memcpy(buffer.data() + size, &objectType, sizeof(ObjectType));
  size += sizeof(ObjectType);
  serializeTransform(buffer.data(), transform, size);
  memcpy(buffer.data() + size, &interactable, sizeof(bool));
  size += sizeof(bool);
  memcpy(buffer.data() + size, &active, sizeof(bool));
  return buffer;
}

ObjectPacket ObjectPacket::deserialize(const vector<char> &payload) {
  int objectID;
  ObjectType objectType;
  Transform transform;
  bool interactable;
  bool active;

  unsigned long size = 0;
  memcpy(&objectID, payload.data(), sizeof(int));
  size += sizeof(int);
  memcpy(&objectType, payload.data() + size, sizeof(ObjectType));
  size += sizeof(ObjectType);
  deserializeTransform(payload, transform, size);
  memcpy(&interactable, payload.data() + size, sizeof(bool));
  size += sizeof(bool);
  memcpy(&active, payload.data() + size, sizeof(bool));
  ObjectPacket packet(objectID, objectType, transform);
  return packet;
}

vector<char> MovementPacket::serialize() const {
  vector<char> buffer(sizeof(int) + sizeof(MovementType));
  memcpy(buffer.data(), &objectID, sizeof(int));
  memcpy(buffer.data() + sizeof(int), &movementType, sizeof(MovementType));
  return buffer;
}

MovementPacket MovementPacket::deserialize(const vector<char> &payload) {
  int objectID;
  MovementType movementType;

  memcpy(&objectID, payload.data(), sizeof(int));
  memcpy(&movementType, payload.data() + sizeof(int), sizeof(MovementType));
  MovementPacket packet(objectID, movementType);
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

std::unique_ptr<IPacket> deserialize(PacketType type, vector<char> &payload) {
  switch (type) {
  case PacketType::INIT:
    return make_unique<InitPacket>(InitPacket::deserialize(payload));
  case PacketType::OBJECT:
    return make_unique<ObjectPacket>(ObjectPacket::deserialize(payload));
  case PacketType::MOVEMENT:
    return make_unique<MovementPacket>(MovementPacket::deserialize(payload));
  case PacketType::DISCONNECT:
    return make_unique<DisconnectPacket>(
        DisconnectPacket::deserialize(payload));
  default:
    throw runtime_error("Unknown packet type");
  }
}

// Debugging
void printObjectPacket(const ObjectPacket &packet) {
  std::cout << "ObjectPacket: " << std::endl;
  std::cout << "id: " << packet.objectID << std::endl;
  std::cout << "type: " << int(packet.objectType) << std::endl;
  std::cout << "position: (" << packet.transform.getPosition().x << ", "
            << packet.transform.getPosition().y << ", "
            << packet.transform.getPosition().z << ")" << std::endl;
  std::cout << "rotation: (" << packet.transform.getRotation().x << ", "
            << packet.transform.getRotation().y << ", "
            << packet.transform.getRotation().z << ")" << std::endl;
  std::cout << "scale: (" << packet.transform.getScale().x << ", "
            << packet.transform.getScale().y << ", "
            << packet.transform.getScale().z << ")" << std::endl;
  std::cout << "interactable: " << packet.interactable << std::endl;
  std::cout << "active: " << packet.active << std::endl;
}