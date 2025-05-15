#pragma once

#include <stdint.h>

#define NUM_COLUMNS_CSV 15

#define JSON_PATH "../resources/objects/objects.json"

enum class MovementType : uint8_t { FORWARD, BACKWARD, LEFT, RIGHT, NONE };

enum class InteractionType : uint8_t {
  NONE,
  PICKUP,
  PRESS,
  OPEN_CLOSE,
};