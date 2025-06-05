#pragma once

#include "globals.hpp"
#include "puzzle.hpp"

#include <algorithm>
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

class Level {
private:
  LevelType level;
  NUM_PUZZLE numMilestones;
  PUZZLE_ID currentMilestone;
  unordered_map<PUZZLE_ID, unique_ptr<Puzzle>>
      clues; // puzzles that provide clues or guidance
  unordered_map<PUZZLE_ID, unique_ptr<Puzzle>>
      milestones; // puzzles that must be completed to advance the level
  vector<pair<RewardType, vector<OBJECT_ID>>> rewards;

public:
  Level(LevelType l) : level(l), numMilestones(0), currentMilestone(0){};
  void addCluePuzzle(PUZZLE_ID id, unique_ptr<Puzzle> puzzle);
  void addMilestonePuzzle(unique_ptr<Puzzle> puzzle);
  bool isLevelComplete();
  vector<pair<RewardType, vector<OBJECT_ID>>> getPuzzleRewards();
};

class LevelManager {
private:
  Level *currentLevel;
  LevelType currentLevelType;
  unordered_map<LevelType, unordered_map<OBJECT_ID, GameObject *>> levelObjects;
  unordered_map<LevelType, unique_ptr<Level>> levels;

public:
  // change to NONE when level should start before beginning level
  LevelManager() : currentLevel(nullptr), currentLevelType(LevelType::BARN) {}

  void addObject(LevelType levelType, OBJECT_ID objectID, GameObject *object);
  void addLevel(LevelType levelType, unique_ptr<Level> level);
  void loadJSON();
  bool updateLevels();
  void advanceLevel();
  vector<pair<RewardType, vector<OBJECT_ID>>> getRewards();
  void instantiatePlayers();

  LevelType getLevel() const { return currentLevelType; }

  glm::vec3 startingPositions[NUM_PLAYERS] = {
      glm::vec3(0.0f, 0.0f, 0.0f),   // Player 1
      glm::vec3(5.0f, 0.0f, 5.0f),   // Player 2
      glm::vec3(-5.0f, 0.0f, -5.0f), // Player 3
      glm::vec3(-5.0f, 0.0f, -5.0f)  // Player 4
  };
};