#include "utilities/util_packets.hpp"

void serializeVector(char *dest, const glm::vec3 &vec3, unsigned long &size) {
  memcpy(dest + size, &vec3.x, sizeof(float));
  size += sizeof(float);
  memcpy(dest + size, &vec3.y, sizeof(float));
  size += sizeof(float);
  memcpy(dest + size, &vec3.z, sizeof(float));
  size += sizeof(float);
  return;
}

glm::vec3 deserializeVector(const vector<char> &payload,
                            unsigned long &size) {
  glm::vec3 vec;
  memcpy(&vec.x, payload.data() + size, sizeof(float));
  size += sizeof(float);
  memcpy(&vec.y, payload.data() + size, sizeof(float));
  size += sizeof(float);
  memcpy(&vec.z, payload.data() + size, sizeof(float));
  size += sizeof(float);
  return vec;
}

void serializeTransform(char *dest, const Transform &transform,
                        unsigned long &size) {
  serializeVector(dest, transform.getPosition(), size);
  serializeVector(dest, transform.getRotation(), size);
  serializeVector(dest, transform.getScale(), size);
  return;
}

void deserializeTransform(const vector<char> &payload, Transform &transform,
                          unsigned long &size) {
  transform.setPosition(
      deserializeVector(payload, size));
  transform.setRotation(
      deserializeVector(payload, size));
  transform.setScale(deserializeVector(payload, size));
  return;
}