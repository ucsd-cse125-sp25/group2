#include "shared/packets.hpp"

using namespace std;

vector<char> InitPacket::serialize() const {
  vector<char> buffer(sizeof(client_id));
  memcpy(buffer.data(), &client_id, sizeof(client_id));
  return buffer;
}

InitPacket InitPacket::deserialize(const vector<char> &payload) {
  int id;
  memcpy(&id, payload.data(), sizeof(int));
  InitPacket packet(id);
  return packet;
}

vector<char> ActionPacket::serialize() const {
  vector<char> buffer(sizeof(int) + sizeof(ActionType));
  memcpy(buffer.data(), &objectID, sizeof(int));
  memcpy(buffer.data() + sizeof(int), &type, sizeof(ActionType));
  return buffer;
}

ActionPacket ActionPacket::deserialize(const vector<char> &payload) {
  int objectID;
  ActionType type;
  memcpy(&objectID, payload.data(), sizeof(int));
  memcpy(&type, payload.data() + sizeof(int), sizeof(ActionType));
  ActionPacket packet(objectID, type);
  return packet;
}

vector<char> ObjectPacket::serialize() const {

  vector<char> buffer(sizeof(int) + sizeof(ObjectType) + sizeof(glm::vec3));

  unsigned long size = 0;

  memcpy(buffer.data(), &id, sizeof(int));
  size += sizeof(int);
  memcpy(buffer.data() + size, &type, sizeof(ObjectType));
  size += sizeof(ObjectType);
  serializeTransform(buffer.data(), transform, size);
  memcpy(buffer.data() + size, &interactable, sizeof(bool));
  size += sizeof(bool);
  memcpy(buffer.data() + size, &active, sizeof(bool));
  return buffer;
}

ObjectPacket ObjectPacket::deserialize(const vector<char> &payload) {
  int id;
  ObjectType type;
  Transform transform;
  bool interactable;
  bool active;

  unsigned long size = 0;
  memcpy(&id, payload.data(), sizeof(int));
  size += sizeof(int);
  memcpy(&type, payload.data() + size, sizeof(ObjectType));
  size += sizeof(ObjectType);
  deserializeTransform(payload, transform, size);
  memcpy(&interactable, payload.data() + size, sizeof(bool));
  size += sizeof(bool);
  memcpy(&active, payload.data() + size, sizeof(bool));

  ObjectPacket packet(id, type, transform);
  return packet;
}

vector<char> DisconnectPacket::serialize() const {
  vector<char> buffer(sizeof(client_id));
  memcpy(buffer.data(), &client_id, sizeof(client_id));
  return buffer;
}

DisconnectPacket DisconnectPacket::deserialize(const vector<char> &payload) {
  int id;
  memcpy(&id, payload.data(), sizeof(int));
  DisconnectPacket packet(id);
  return packet;
}

vector<char> PositionPacket::serialize() const {

  vector<char> buffer(sizeof(int) + sizeof(ObjectType) + sizeof(glm::vec3));

  unsigned long size = 0;

  memcpy(buffer.data(), &object_id, sizeof(int));
  size += sizeof(int);
  serializeTransform(buffer.data(), transform, size);
  return buffer;
}

PositionPacket PositionPacket::deserialize(const vector<char> &payload) {
  int id;
  Transform transform;

  unsigned long size = 0;
  memcpy(&id, payload.data(), sizeof(int));
  size += sizeof(int);
  deserializeTransform(payload, transform, size);

  PositionPacket packet(id, transform);
  return packet;
}

std::unique_ptr<IPacket> deserialize(PacketType type, vector<char> &payload) {
  switch (type) {
  case PacketType::INIT:
    return make_unique<InitPacket>(InitPacket::deserialize(payload));
  case PacketType::OBJECT:
    return make_unique<ObjectPacket>(ObjectPacket::deserialize(payload));
  case PacketType::POSITION:
    return make_unique<PositionPacket>(PositionPacket::deserialize(payload));
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
  std::cout << "id: " << packet.id << std::endl;
  std::cout << "type: " << int(packet.type) << std::endl;
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

void printPositionPacket(const PositionPacket &packet) {
  std::cout << "PositionPacket: " << std::endl;
  std::cout << "id: " << packet.object_id << std::endl;
  std::cout << "position: (" << packet.transform.getPosition().x << ", "
            << packet.transform.getPosition().y << ", "
            << packet.transform.getPosition().z << ")" << std::endl;
  std::cout << "rotation: (" << packet.transform.getRotation().x << ", "
            << packet.transform.getRotation().y << ", "
            << packet.transform.getRotation().z << ")" << std::endl;
  std::cout << "scale: (" << packet.transform.getScale().x << ", "
            << packet.transform.getScale().y << ", "
            << packet.transform.getScale().z << ")" << std::endl;
}