#pragma once

#include "client/shader.hpp"
#include "shared/gameobject.hpp"
#include <memory>
#include <vector>

using namespace std;
class GameState {
public:
  GameState();

  bool init();
  void update();
  void draw(const glm::mat4 &viewProjMtx);

private:
  vector<unique_ptr<GameObject>> objectList;
  // delete later
  Cube *cube;
};
