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
  vector<char> buffer(sizeof(int) + sizeof(ObjectType) + sizeof(Transform) +
                      2 * sizeof(bool));
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
  packet.active = active;
  packet.interactable = interactable;
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
  case PacketType::DISCONNECT:
    return make_unique<DisconnectPacket>(
        DisconnectPacket::deserialize(payload));
  default:
    throw runtime_error("Unknown packet type");
  }
}

// Debugging
void printObjectPacket(const ObjectPacket &packet) {
  cout << "ObjectPacket: " << endl;
  cout << "id: " << packet.objectID << endl;
  cout << "type: " << int(packet.objectType) << endl;
  cout << "position: (" << packet.transform.getPosition().x << ", "
       << packet.transform.getPosition().y << ", "
       << packet.transform.getPosition().z << ")" << endl;
  cout << "rotation: (" << packet.transform.getRotation().x << ", "
       << packet.transform.getRotation().y << ", "
       << packet.transform.getRotation().z << ")" << endl;
  cout << "scale: (" << packet.transform.getScale().x << ", "
       << packet.transform.getScale().y << ", " << packet.transform.getScale().z
       << ")" << endl;
  cout << "interactable: " << packet.interactable << endl;
  cout << "active: " << packet.active << endl;
}