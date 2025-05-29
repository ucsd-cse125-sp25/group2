#pragma once

#include "globals.hpp"
#include "puzzle.hpp"

#include <fstream>
#include <iostream>
#include <magic_enum/magic_enum.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

using json = nlohmann::json;
using namespace std;

#define PUZZLE_ID int
#define NUM_PUZZLE int
#define NUM_LEVEL int

class Level {
private:
  LEVEL_ID level;
  PUZZLE_ID currentPuzzle;
  NUM_PUZZLE numPuzzles;
  OBJECT_ID rewardObjectID = -1;
  unordered_map<PUZZLE_ID, unique_ptr<Puzzle>> puzzles;

public:
  Level(LEVEL_ID id) : level(id), currentPuzzle(0), numPuzzles(0){};
  void addPuzzle(unique_ptr<Puzzle> puzzle);
  bool isLevelComplete();
  OBJECT_ID getReward();
};

class LevelManager {
private:
  Level *currentLevel = nullptr;
  LEVEL_ID currentLevelID = 0;
  NUM_LEVEL numLevels = 0;
  unordered_map<LEVEL_ID, unordered_map<OBJECT_ID, GameObject *>> levelObjects;
  unordered_map<LEVEL_ID, unique_ptr<Level>> levels;

public:
  LevelManager(){};

  void addObject(LEVEL_ID levelID, OBJECT_ID objectID, GameObject *object) {
    levelObjects[levelID][objectID] = object;
  }
  void addLevel(LEVEL_ID id, unique_ptr<Level> level) {
    levels[id] = move(level);
    numLevels++;
  }
  void loadJSON();
  bool updateLevels();
  void advanceLevel();
  OBJECT_ID getRewardObjectID();
};