#include "level_manager.hpp"

void Level::addPuzzle(unique_ptr<Puzzle> puzzle) {
  puzzles[numPuzzle] = move(puzzle);
  numPuzzle++;
}

bool Level::isLevelComplete() {
  auto puzzle = puzzles[currentPuzzle].get();
  return puzzle->isPuzzleComplete();
}

LevelManager::LevelManager() {

  ifstream file(PUZZLE_PATH);
  if (!file.is_open()) {
    cerr << "Failed to open JSON file: " << PUZZLE_PATH << endl;
  }

  json levelsData;
  try {
    file >> levelsData;
  } catch (const exception &e) {
    cerr << "JSON parsing error: " << e.what() << endl;
    return;
  }

  if (levelsData.contains("levels") && levelsData["levels"].is_array()) {
    for (const auto &levelData : levelsData["levels"]) {
      LEVEL_ID level_id = levelData["level_id"].get<int>();
      unique_ptr<Level> new_level = make_unique<Level>(level_id);
    }
  }
}
