#pragma once

#include "client_gameobject.hpp"
#include "csv.h"
#include "shader.hpp"

#include <memory>
#include <unordered_map>

using namespace std;

class ClientGameState {
public:
  ClientGameState();

  bool init();

  // update methods
  void update(int id, Transform *tf);
  void draw(const glm::mat4 &viewProjMtx);

  GameObject *getObject(int id);

  // delete later
  GameObject *player;
  GameObject *getPlayer() { return player; }

private:
  unordered_map<int, unique_ptr<GameObject>> objectList;
};
