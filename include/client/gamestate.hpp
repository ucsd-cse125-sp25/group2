#pragma once

#include "shared/gameobject.hpp"
#include "shader.hpp"
#include <unordered_map>
#include <vector>

class GameState {
public:
  std::unordered_map<int, bool> keyStates;

  GameState();
  void Update(float deltaTime);
  void Render(const glm::mat4 &viewProjMtx);

  void InitializeGameObject(GameObject *gameObject);
  void KeyboardInput(float deltaTime);

private:
  std::vector<Shader> shaders;
  std::vector<GameObject *> gameObjects;
  std::unordered_map<GameObject *, Transform *> transforms;

  GameObject *player;
};
