#pragma once

#include <stdint.h>

#ifndef GLOBALS_H // Include guard to prevent multiple inclusions
#define GLOBALS_H

#define NUM_COLUMNS_CSV 15

#define JSON_PATH "../resources/objects/objects.json"
#define NUM_COLUMNS_CSV 15

enum class MovementType : uint8_t { FORWARD, BACKWARD, LEFT, RIGHT, NONE };

enum class InteractionType : uint8_t {
  NONE,
  PICKUP,
  PRESS,
  OPEN_CLOSE,
};

#endif // GLOBALS_H