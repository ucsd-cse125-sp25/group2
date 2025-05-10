#pragma once

#include <stdint.h>

#ifndef GLOBALS_H  // Include guard to prevent multiple inclusions
#define GLOBALS_H

#define NUM_COLUMNS_CSV 14

enum class MovementType : uint8_t { FORWARD, BACKWARD, LEFT, RIGHT };

#endif // GLOBALS_H