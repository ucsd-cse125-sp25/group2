#include "level_manager.hpp"

void Level::addPuzzle(unique_ptr<Puzzle> puzzle) {
  puzzles[numPuzzles] = move(puzzle);
  numPuzzles++;
}

bool Level::isLevelComplete() {
  while (currentPuzzle < numPuzzles) {
    Puzzle* puzzle = puzzles[currentPuzzle].get();
    if (puzzle->isPuzzleComplete()){
      updatedObjectIds.push_back(puzzle->dispatchReward());
      currentPuzzle++;
    }
    else  {
      break;
    }
  }

  return (currentPuzzle < numPuzzles);
}

vector<OBJECT_ID> Level::getUpdatedObjects() {
  vector<OBJECT_ID> list(updatedObjectIds.begin(), updatedObjectIds.end());
  updatedObjectIds.clear();
  return list;  
}

void LevelManager::addLevel(LEVEL_ID id, unique_ptr<Level> level) {}

void LevelManager::loadJSON() {
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
      LEVEL_ID id = levelData["levelID"].get<int>();
      auto currentLevelObjects = levelObjects[id];
      unique_ptr<Level> newLevel = make_unique<Level>(id);
      if (levelData.contains("puzzles") && levelData["puzzles"].is_array()) {
        for (const auto &puzzleData : levelData["puzzles"]) {
          OBJECT_ID puzzleObjId = puzzleData["objectID"].get<int>();
          GameObject* puzzleObject = currentLevelObjects[puzzleObjId];
          unique_ptr<Puzzle> newPuzzle = make_unique<Puzzle>(puzzleObject);
          if (puzzleData.contains("conditions") &&
              puzzleData["conditions"].is_array()) {
            for (const auto &conditionData : puzzleData["conditions"]) {
              unique_ptr<PuzzleCondition> condition;
              OBJECT_ID objId = conditionData["objectID"].get<int>();
              GameObject* object = currentLevelObjects[objId];
              string conditionStr =
                  conditionData["conditionType"].get<string>();
              auto conditionVal =
                  magic_enum::enum_cast<ConditionType>(conditionStr);
              if (conditionVal.has_value()) {
                ConditionType conditionType = conditionVal.value();
                switch (conditionType) {
                case ConditionType::PRESSURE_PLATE:
                  condition = make_unique<PressurePlateCondition>(object);
                  break;
                case ConditionType::BUTTON:
                  condition = make_unique<ButtonCondition>(object);
                  break;
                case ConditionType::PIN:
                  condition = make_unique<PinCondition>(object);
                  break;
                }
              }
              newPuzzle->addCondition(move(condition));
            }
          }
          newLevel->addPuzzle(move(newPuzzle));
        }
      }
      addLevel(id, move(newLevel));
    }
  }

  currentLevel = levels[currentLevelID].get();
}

void LevelManager::update() {}

void LevelManager::advanceLevel() {}