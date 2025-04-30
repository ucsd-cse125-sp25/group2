#include "shared/utilities/util_packets.hpp"
#include "shared/core.hpp"
#include "shared/packets.hpp"
#include <iostream>

using namespace std;

void serializeVector(char *dest, const glm::vec3 &vec3, unsigned long &size) {
  memcpy(dest + size, &vec3.x, sizeof(float));
  size += sizeof(float);
  memcpy(dest + size, &vec3.y, sizeof(float));
  size += sizeof(float);
  memcpy(dest + size, &vec3.z, sizeof(float));
  size += sizeof(float);
  return;
}

glm::vec3 deserializeVector(const vector<char> &payload, glm::vec3 vec,
                            unsigned long &size) {
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
      deserializeVector(payload, transform.getPosition(), size));
  transform.setRotation(
      deserializeVector(payload, transform.getRotation(), size));
  transform.setPosition(deserializeVector(payload, transform.getScale(), size));
  return;
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