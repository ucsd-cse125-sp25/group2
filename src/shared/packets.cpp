#include "shared/packets.hpp"
#include "shared/utilities/util_packets.hpp"

using namespace std;

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