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

  json objectsData;
  try {
    file >> objectsData;
  } catch (const exception &e) {
    cerr << "JSON parsing error: " << e.what() << endl;
    return;
  }

  if (objectsData.contains("levels") && objectsData["levels"].is_array()) {
    for (const auto &levelData: objectsData["levels"]) {
      LEVEL_ID level_id = stoi(objData['level_id'])
      unique_ptr<Level> new_level = make_unique(Level(level_id));

    }
  }
}
