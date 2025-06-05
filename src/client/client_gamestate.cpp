#include "client_gamestate.hpp"

bool ClientGameState::init() {
  ObjectLoader objectLoader = ObjectLoader();
  objectList = objectLoader.loadObjects();
  notes = objectLoader.loadNotes();
  objectLoader.loadLights();
  for (auto &obj : objectList) {
    auto id = obj.first;
    auto object = obj.second.get();
    levelObjects[object->getLevel()][id] = object;
  }
  state = Gamestate::STARTSCREEN;
  return true;
}

void ClientGameState::advanceLevel(LevelType newLevel) {
  uint8_t levelNum = magic_enum::enum_integer(newLevel);
  if (levelNum <= NUM_LEVELS) {
    state = Gamestate::LOADING;
    UIManager::loadingScreen->play();
  } else {
    state = Gamestate::COMPLETED;
  }

  int id = 0;
  for (auto &obj : levelObjects[LevelType::ALL]) {
    obj.second->activate();
    uint8_t levelNum = magic_enum::enum_integer(obj.second->getLevel());
    if (levelNum == 1) {
      obj.second->getTransform()->setPosition(STARTING_POSITIONS[id]);
      id++;
    } else {
      obj.second->getTransform()->setPosition(obj.second->getOriginalPosition());
    }
  }

  // Deactivate all objects in the current level
  for (auto &obj : levelObjects[currentLevelType]) {
    obj.second->deactivate();
  }

  currentLevelType = newLevel; // update current level
  LightManager::setCurrentLevel(currentLevelType);

  // Activate all objects in the new level
  for (auto &obj : levelObjects[currentLevelType]) {
    if (notes.find(obj.first) != notes.end()) {
      continue; // Skip notes, want to keep them deactivated
    }
    obj.second->activate();
  }
}

void ClientGameState::update(OBJECT_ID id, Transform *tf) {
  auto obj = getObject(id);
  if (obj && obj->isActive())
    obj->update(tf);
}

void ClientGameState::draw(const glm::mat4 &viewProjMtx, const glm::vec3 &pos) {
  // Draw all objects
  for (auto it = objectList.begin(); it != objectList.end(); it++) {
    auto &object = it->second;
    if (object->isActive()) {
      object->draw(viewProjMtx, pos);
    }
  }
}

GameObject *ClientGameState::getObject(OBJECT_ID id) {
  auto itr = objectList.find(id);
  if (itr != objectList.end()) {
    return itr->second.get();
  }
  cerr << "Object with id " << id << " not found" << endl;
  return nullptr;
}