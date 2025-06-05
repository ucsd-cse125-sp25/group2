#pragma once
#include "core.hpp"

#include <stdint.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define CLIENT_ID int
#define OBJECT_ID int
#define PLAYER_ID int

#define NUM_PLAYERS 4
#define NUM_LEVELS 4

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
  KEYPAD,
};

enum class RewardType : uint8_t {
  NONE,
  ACTIVATE,
  DEACTIVATE,
};

// for testing until we add more level logic
enum class LevelType : uint8_t {
  NONE,
  PENS,
  WINDMILL,
  SILO,
  BARN,
  ALL,
};

static const glm::vec3 STARTING_POSITIONS[NUM_PLAYERS] = {
    glm::vec3(-82.0f, 0.865999996f, 15.0f),   // Player 1
    glm::vec3(58.594f, 0.865999996f, 93.0f),  // Player 2
    glm::vec3(32.361f, 0.865999996f, -12.0f),   // Player 3
    glm::vec3(-76.0f, 0.865999996f, 105.0f)    // Player 4
};