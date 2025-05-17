#pragma once

#include <stdint.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define CLIENT_ID unsigned int

#define JSON_PATH "../resources/objects/objects.json"

enum class MovementType : uint8_t { FORWARD, BACKWARD, LEFT, RIGHT, NONE };

enum class InteractionType : uint8_t {
  NONE,
  PICKUP,
  PRESS,
  OPEN_CLOSE,
};