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
  NUM_PUZZLE numMilestones;
  PUZZLE_ID currentMilestone;
  unordered_map<PUZZLE_ID, unique_ptr<Puzzle>> clues; // puzzles that provide clues or guidance
  unordered_map<PUZZLE_ID, unique_ptr<Puzzle>> milestones;  // puzzles that must be completed to advance the level
  vector<pair<RewardType, vector<OBJECT_ID>>> rewards;

public:
  Level(LEVEL_ID id) : level(id), numMilestones(0), currentMilestone(0) {};
  void addCluePuzzle(PUZZLE_ID id, unique_ptr<Puzzle> puzzle);
  void addMilestonePuzzle(unique_ptr<Puzzle> puzzle);
  bool isLevelComplete();
  vector<pair<RewardType, vector<OBJECT_ID>>> getPuzzleRewards();
};

class LevelManager {
private:
  Level *currentLevel;
  LEVEL_ID currentLevelID;
  NUM_LEVEL numLevels;
  unordered_map<LEVEL_ID, unordered_map<OBJECT_ID, GameObject *>> levelObjects;
  unordered_map<LEVEL_ID, unique_ptr<Level>> levels;

public:
  LevelManager(): currentLevel(nullptr), currentLevelID(0), numLevels(0) {}

  void addObject(LEVEL_ID levelID, OBJECT_ID objectID, GameObject *object);
  void addLevel(LEVEL_ID id, unique_ptr<Level> level);
  void loadJSON();
  bool updateLevels();
  void advanceLevel();
  vector<pair<RewardType, vector<OBJECT_ID>>> getRewards();
};