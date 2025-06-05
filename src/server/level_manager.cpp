#include "level_manager.hpp"

void Level::addCluePuzzle(PUZZLE_ID id, unique_ptr<Puzzle> puzzle) {
  clues[id] = move(puzzle);
}

void Level::addMilestonePuzzle(unique_ptr<Puzzle> puzzle) {
  milestones[numMilestones] = move(puzzle);
  numMilestones++;
}

void Level::addNote(OBJECT_ID id) { notes.push_back(id); }

bool Level::isLevelComplete() {
  // loop through all clue puzzles, if complete dispatch reward (activate or
  // deactivate)
  for (const auto &cluePair : clues) {
    auto clue = cluePair.second.get();
    if (clue->isPuzzleComplete())
      rewards.push_back(clue->dispatchReward());
  }

  // loop through milestones, which are puzzles that must be completed for the
  // level to be complete milestone puzzles have specific order of completion
  Puzzle *milestone = nullptr;
  if (currentMilestone <= numMilestones) {
    milestone = milestones[currentMilestone].get();
    if (milestone->isPuzzleComplete()) {
      rewards.push_back(milestone->dispatchReward());
      currentMilestone++;
    }
  }

  return (currentMilestone == numMilestones);
}

vector<pair<RewardType, vector<OBJECT_ID>>> Level::getPuzzleRewards() {
  vector<pair<RewardType, vector<OBJECT_ID>>> res = rewards;
  rewards.clear();
  return res;
}

vector<OBJECT_ID> Level::getNotes() { return notes; }

void LevelManager::addObject(LevelType levelType, OBJECT_ID objectID,
                             GameObject *object) {
  levelObjects[levelType][objectID] = object;
}

void LevelManager::addLevel(LevelType levelType, unique_ptr<Level> level) {
  levels[levelType] = move(level);
}

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
      auto levelTypeVal = magic_enum::enum_cast<LevelType>(
          levelData["levelType"].get<string>());
      LevelType levelType = levelTypeVal.value_or(LevelType::NONE);
      auto currentLevelObjects = levelObjects[levelType];
      unique_ptr<Level> newLevel = make_unique<Level>(levelType);

      if (levelData.contains("puzzles") && levelData["puzzles"].is_array()) {
        for (const auto &puzzleData : levelData["puzzles"]) {
          PUZZLE_ID puzzleID = puzzleData["puzzleID"].get<int>();

          string rewardTypeStr = puzzleData["rewardType"].get<string>();
          auto rewardTypeVal = magic_enum::enum_cast<RewardType>(rewardTypeStr);
          RewardType rewardType = rewardTypeVal.value_or(RewardType::NONE);

          vector<OBJECT_ID> rewardIDs;
          for (const auto &rewardID : puzzleData["rewardIDs"]) {
            if (rewardType == RewardType::ACTIVATE) {
              newLevel->addNote(rewardID.get<int>());
            }
            rewardIDs.push_back(rewardID.get<int>());
          }

          unique_ptr<Puzzle> newPuzzle =
              make_unique<Puzzle>(rewardType, rewardIDs);

          if (puzzleData.contains("conditions") &&
              puzzleData["conditions"].is_array()) {
            for (const auto &conditionData : puzzleData["conditions"]) {
              unique_ptr<PuzzleCondition> condition;
              OBJECT_ID objID = conditionData["objectID"].get<int>();
              GameObject *object = currentLevelObjects[objID];
              string conditionStr =
                  conditionData["conditionType"].get<string>();
              auto conditionVal =
                  magic_enum::enum_cast<ConditionType>(conditionStr);

              if (conditionVal.has_value()) {
                ConditionType conditionType = conditionVal.value();
                switch (conditionType) {
                case ConditionType::PRESSURE_PLATE: {
                  OBJECT_ID triggeringObjectID =
                      conditionData["triggeringObjectID"].get<int>();
                  condition = make_unique<PressurePlateCondition>(
                      object, triggeringObjectID);
                  break;
                }
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
          string puzzleType = puzzleData["puzzleType"].get<string>();
          if (puzzleType == "milestone") {
            newLevel->addMilestonePuzzle(move(newPuzzle));
          } else if (puzzleType == "clue") {
            newLevel->addCluePuzzle(puzzleID, move(newPuzzle));
          }
        }
      }
      addLevel(levelType, move(newLevel));
    }
  }

  currentLevel = levels[currentLevelType].get();
}

bool LevelManager::updateLevels() { return currentLevel->isLevelComplete(); }

void LevelManager::advanceLevel() {
  if (currentLevelType == LevelType::NONE) {
    instantiatePlayers();
  }

  // Deactivate all objects in the current level
  for (const auto &objPair : levelObjects[currentLevelType]) {
    GameObject *object = objPair.second;
    object->deactivate();
  }

  uint8_t levelNum = magic_enum::enum_integer(currentLevelType);
  levelNum++;
  cout << "Advancing level to: " << static_cast<int>(levelNum) << endl;

  currentLevelType =
      magic_enum::enum_cast<LevelType>(levelNum).value_or(LevelType::NONE);
  currentLevel = levels[currentLevelType].get();

  if (levelNum > NUM_LEVELS) {
    cout << "All levels completed!" << endl;
    return;
  }
  // get the objects that we don't want to activate when the level changes,
  // i.e. notes
  vector<OBJECT_ID> rewardIDs = currentLevel->getNotes();

  // Activate all objects except for notes in the new level
  for (const auto &objPair : levelObjects[currentLevelType]) {
    GameObject *object = objPair.second;
    if (find(rewardIDs.begin(), rewardIDs.end(), object->getID()) !=
        rewardIDs.end()) {
      continue;
    }
    object->activate();
  }

}

vector<pair<RewardType, vector<OBJECT_ID>>> LevelManager::getRewards() {
  return currentLevel->getPuzzleRewards();
}

void LevelManager::instantiatePlayers() {
  auto animals = levelObjects[LevelType::ALL];
  for (const auto &animalPair : animals) {
    GameObject *animal = animalPair.second;
    animal->activate();
    animal->getTransform()->setPosition(startingPositions[animal->getID()]);
  }
}