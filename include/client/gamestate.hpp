#pragma once

#include "shader.hpp"
#include "shared/gameobject.hpp"
#include <unordered_map>
#include <vector>

using namespace std;

class GameState {
public:
  std::unordered_map<int, bool> keyStates;

  GameState();
  void Update(float deltaTime);
  void Render(const glm::mat4 &viewProjMtx);

  void InitializeGameObject(GameObject *gameObject);
  void KeyboardInput(float deltaTime);

private:
  std::vector<GameObject *> gameObjects;
  std::unordered_map<GameObject *, Transform *> transforms;

  GameObject *player;
};
