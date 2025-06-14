#pragma once

#include "core.hpp"
#include "transform.hpp"

#include <cstring>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <vector>

using namespace std;

void serializeVector(char *dest, const glm::vec3 &vec3, unsigned long &size);

glm::vec3 deserializeVector(const vector<char> &payload, unsigned long &size);

void serializeTransform(char *dest, const Transform &transform,
                        unsigned long &size);

void deserializeTransform(const vector<char> &payload, Transform &transform,
                          unsigned long &size);