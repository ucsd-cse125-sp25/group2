#pragma once

#include "client/shader.hpp"
#include "csv.h"
#include "shared/gameobject.hpp"
#include "shared/objects/cube.hpp"

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

private:
  unordered_map<int, unique_ptr<GameObject>> objectList;
};
