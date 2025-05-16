#pragma once

#include <stdint.h>

#define JSON_PATH "../resources/objects/objects.json"

enum class MovementType : uint8_t { FORWARD, BACKWARD, LEFT, RIGHT };

enum class InteractionType : uint8_t {
  NONE,
  PICKUP,
  PRESS,
  OPEN_CLOSE,
};