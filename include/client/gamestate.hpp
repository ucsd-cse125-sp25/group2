#pragma once

#include "shader.hpp"
#include "shared/gameobject.hpp"
#include "shared/utilities/pch.hpp"

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
