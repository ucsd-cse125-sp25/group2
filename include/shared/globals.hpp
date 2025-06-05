#pragma once

#include <stdint.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define CLIENT_ID int
#define OBJECT_ID int
#define PLAYER_ID int

#define NUM_PLAYERS 4
#define NUM_LEVELS 1

#define CHICKEN 0
#define PIG 1
#define SHEEP 2
#define COW 3

#define OBJECT_PATH "../resources/objects/objects.json"
#define PUZZLE_PATH "../resources/puzzles/puzzles.json"
#define CONFIG_PATH "../resources/config.json"
#define NOTE_PATH "../resources/ui/notes.json"
#define LIGHT_PATH "../resources/objects/lights.json"

enum class Gamestate : uint8_t {
  STARTSCREEN,
  MAINMENU,
  READY,
  GAME,
  LOADING,
  COMPLETED
};

enum class MovementType : uint8_t {
  NONE,
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  JUMP,
  GLIDE,
  RESET
};

enum class InteractionType : uint8_t {
  NONE,
  PICKUP,
  PRESS,
  OPEN_CLOSE,
  KEYPAD,
};

enum class RewardType : uint8_t {
  NONE,
  ACTIVATE,
  DEACTIVATE,
};

// for testing until we add more level logic
// enum class LevelType : uint8_t {
//   NONE,
//   BARN,

//   ALL,
// };

enum class LevelType : uint8_t {
  NONE,
  PENS,
  SILO,
  WINDMILL,
  BARN,
  ALL,
};