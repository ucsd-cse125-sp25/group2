#include "client/gamestate.hpp"

// delete later
#include "shared/objects/cube.hpp"

using namespace std;

GameState::GameState() {}

bool GameState::init() {
  // Initialize objects
  cube = new Cube();
  return true;
  // Later: objectList.push_back(make_unique<>());
}

void GameState::update() {
  // Update all objects
  // for (auto &object : objectList) {
  //   object->update(deltaTime);
  // }
  cube->update();
}

void GameState::draw(const glm::mat4 &viewProjMtx) {
  // Draw all objects
  // for (auto &object : objectList) {
  //   object->draw(viewProjMtx);
  // }

  // delete later
  unique_ptr<Shader> cubeShader =
      make_unique<Shader>(Shader("../src/client/shaders/shader.vert",
                                 "../src/client/shaders/shader.frag"));
  cube->draw(viewProjMtx, cubeShader);
}