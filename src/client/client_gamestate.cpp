#include "client_gamestate.hpp"

bool ClientGameState::init() {
  ObjectLoader objectLoader = ObjectLoader();
  objectList = objectLoader.loadObjects();
  notes = objectLoader.loadNotes();
  for (auto &obj : objectList) {
    auto id = obj.first;
    auto object = obj.second.get();
    levelObjects[object->getLevelID()][id] = object;
  }
  state = Gamestate::STARTSCREEN;
  return true;
}

void ClientGameState::changeLevel(LEVEL_ID levelNum) {

  if (levelNum < NUM_LEVELS - 1) {
    state = Gamestate::LOADING;
    UIManager::loadingScreen->play();
  } else {
    state = Gamestate::COMPLETED;
  }
  cout << "Changing level to: " << levelNum << endl;

  // Deactivate all objects in the current level
  for (auto &obj : levelObjects[level]) {
    obj.second->deactivate();
  }
  level = levelNum;
  // Activate all objects in the new level
  for (auto &obj : levelObjects[level]) {
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