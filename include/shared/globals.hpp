#pragma once

#include <stdint.h>

#ifndef GLOBALS_H // Include guard to prevent multiple inclusions
#define GLOBALS_H

#define NUM_COLUMNS_CSV 15

#define JSON_PATH "../resources/objects/objects.json"
#define NUM_COLUMNS_CSV 15

#define JSON_PATH "../resources/objects/objects.json"

enum class MovementType : uint8_t
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

enum class InteractionType : uint8_t
{
    NONE,
    PICKUP,
    PRESS,
    OPEN_CLOSE,
};

#endif // GLOBALS_H