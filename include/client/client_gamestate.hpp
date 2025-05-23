#pragma once

#include "client_gameobject.hpp"
#include "client_object_loader.hpp"
#include "globals.hpp"

#include <memory>
#include <unordered_map>

using namespace std;

class ClientGameState {
private:
  unordered_map<int, unique_ptr<GameObject>> objectList;
  GameObject *player;

public:
  Gamestate state;

  bool init();

  // update methods
  void update(int id, Transform *tf);
  void draw(const glm::mat4 &viewProjMtx);

  GameObject *getObject(int id);
  GameObject *getPlayer() { return player; }
};