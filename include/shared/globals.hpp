#pragma once

#include <stdint.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define CLIENT_ID int
#define OBJECT_ID int
#define PLAYER_ID int

#define NUM_PLAYERS 4

#define CHICKEN 0
#define PIG 1
#define SHEEP 2
#define COW 3

#define JSON_PATH "../resources/objects/objects.json"
#define CONFIG_PATH "../resources/config.json"

enum class MovementType : uint8_t {
  NONE,
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  JUMP,
  GLIDE
};

enum class InteractionType : uint8_t {
  NONE,
  PICKUP,
  PRESS,
  OPEN_CLOSE,
  KEYPAD,
  NOTE,
};

enum class Gamestate : uint8_t { STARTSCREEN, MAINMENU, READY, GAME };