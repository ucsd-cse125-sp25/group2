#pragma once

#include "globals.hpp"
#include "puzzle.hpp"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

using json = nlohmann::json;
using namespace std;

#define PUZZLE_ID int
#define LEVEL_ID int
#define NUM_PUZZLE int

class Level {
private:
  LEVEL_ID level;
  PUZZLE_ID currentPuzzle;
  NUM_PUZZLE numPuzzle;
  unordered_map<PUZZLE_ID, unique_ptr<Puzzle>> puzzles;

public:
  Level(LEVEL_ID id) : level(id), currentPuzzle(0), numPuzzle(0){};
  void addPuzzle(unique_ptr<Puzzle> puzzle);
  void removePuzzle(int id);
  bool isLevelComplete();
};

class LevelManager {
private:
  Level *currentLevel = nullptr;
  LEVEL_ID current_level_id = 0;
  unordered_map<LEVEL_ID, unique_ptr<Level>> levels;

public:
  LevelManager();

  void loadLevel(LEVEL_ID id);
  void update();
  void advanceLevel();
};