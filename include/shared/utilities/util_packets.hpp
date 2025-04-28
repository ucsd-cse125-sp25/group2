#pragma once
#include "shared/core.hpp"
#include <vector>
#include <stdexcept>
#include <stdint.h>
#include <cstring>
#include "shared/transform.hpp"
#include "shared/packets.hpp"
#include <memory>
#include <string>

using namespace std;

void serializeVector(char *dest, const glm::vec3 &vec3, unsigned long &size);

glm::vec3 deserializeVector(const vector<char> &payload, glm::vec3 vec, unsigned long &size);

void serializeTransform(char *dest, const Transform &transform, unsigned long &size);

void deserializeTransform(const vector<char> &payload, Transform &transform, unsigned long &size);

// debugging methods

void printObjectPacket(const ObjectPacket &packet);