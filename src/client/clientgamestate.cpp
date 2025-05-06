#include "client/clientgamestate.hpp"
// delete later
#include "shared/objects/cube.hpp"

ClientGameState::ClientGameState() {}

bool ClientGameState::init() {
  // Initialize objects
  cube = new Cube();
  return true;
  // Later: objectList.push_back(make_unique<>());
}

void ClientGameState::update() {
  // Update all objects
  // for (auto &object : objectList) {
  //   object->update(deltaTime);
  // }
  cube->update();
}

void ClientGameState::draw(const glm::mat4 &viewProjMtx) {
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