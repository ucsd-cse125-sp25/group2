#pragma once

#include "client_gameobject.hpp"
#include "client_object_loader.hpp"
#include "globals.hpp"

#include <memory>
#include <unordered_map>

using namespace std;

class ClientGameState {
private:
  LEVEL_ID level = 0;
  unordered_map<OBJECT_ID, unique_ptr<GameObject>> objectList;
  unordered_map<LEVEL_ID, unordered_map<OBJECT_ID, GameObject *>> levelObjects;
  GameObject *player;

public:
  Gamestate state;

  bool init();

  // update methods
  void changeLevel(LEVEL_ID levelNum);
  void update(OBJECT_ID id, Transform *tf);
  void draw(const glm::mat4 &viewProjMtx);

  GameObject *getObject(OBJECT_ID id);
  GameObject *getPlayer() { return player; }
};