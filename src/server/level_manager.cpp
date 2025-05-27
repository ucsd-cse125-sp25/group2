#include "level_manager.hpp"

void Level::addPuzzle(unique_ptr<Puzzle> puzzle) {
  puzzles[numPuzzle] = move(puzzle);
  numPuzzle++;
}

bool Level::isLevelComplete() {
  auto puzzle = puzzles[currentPuzzle].get();
  return puzzle->isPuzzleComplete();
}

LevelManager::LevelManager() {}
