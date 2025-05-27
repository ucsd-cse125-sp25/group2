#pragma once

#include "puzzle.hpp"

#include <unordered_map>
#include <vector>

using namespace std;

#define PUZZLE_ID int
#define LEVEL_ID int

class Level {
private:
  LEVEL_ID level;
  PUZZLE_ID currentPuzzle;
  unordered_map<PUZZLE_ID, unique_ptr<Puzzle>> puzzles;

public:
  Level(LEVEL_ID id) : level(id), currentPuzzle(0){};
  void addPuzzle(unique_ptr<Puzzle> puzzle);
  void removePuzzle(int id);
  bool isLevelComplete() const;
};

class LevelManager {
private:
  Level *currentLevel;
  unordered_map<LEVEL_ID, unique_ptr<Level>> levels;

public:
  LevelManager(){};

  void loadLevel(LEVEL_ID id);
  void update();
  void advanceLevel();
};