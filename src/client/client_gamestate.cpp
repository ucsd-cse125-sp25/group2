#include "client_gamestate.hpp"

bool ClientGameState::init() {
  ObjectLoader objectLoader = ObjectLoader();
  objectList = objectLoader.loadObjects();
  state = Gamestate::STARTSCREEN;
  return true;
}

void ClientGameState::update(OBJECT_ID id, Transform *tf) {
  auto obj = getObject(id);
  if (obj)
    obj->update(tf);
}

void ClientGameState::draw(const glm::mat4 &viewProjMtx) {
  // Draw all objects
  for (auto it = objectList.begin(); it != objectList.end(); it++) {
    auto &object = it->second;
    object->draw(viewProjMtx);
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