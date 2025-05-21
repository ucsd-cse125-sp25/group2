#pragma once

#include <stdint.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define CLIENT_ID unsigned int
#define OBJECT_ID unsigned int
#define PLAYER_ID unsigned int


#define JSON_PATH "../resources/objects/objects.json"
#define CONFIG_PATH "../resources/config.json"

#define NUM_PLAYERS 4

#define CHICKEN 0
#define SHEEP 1
#define PIG 2
#define COW 3

enum class MovementType : uint8_t { FORWARD, BACKWARD, LEFT, RIGHT, NONE };

enum class InteractionType : uint8_t {
  NONE,
  PICKUP,
  PRESS,
  OPEN_CLOSE,
};

enum class Gamestate : uint8_t { STARTSCREEN, MAINMENU, READY, GAME };