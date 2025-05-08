#pragma once

#include <stdint.h>

enum class PacketType : uint8_t { INIT, OBJECT, MOVEMENT, DISCONNECT };

enum class ObjectType : uint8_t { CUBE };

enum class MovementType : uint8_t { FORWARD, BACKWARD, LEFT, RIGHT };