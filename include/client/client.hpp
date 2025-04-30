#pragma once

#include "client/camera.hpp"
#include "client/clientnetwork.hpp"
#include "client/gamestate.hpp"
#include "client/model.hpp"
#include "client/renderer.hpp"
#include "client/shader.hpp"
#include "shared/core.hpp"
#include "shared/objects/cube.hpp"
#include "shared/physics.hpp"

#define FPS (1.0 / 60.0)

using namespace std;

class Client {
public:
  // Window properties
  GLFWwindow *window;
  int width;
  int height;

  // Constructors and desctructors
  Client();
  ~Client();

  bool init();
  bool initObjects();
  bool initNetwork(asio::io_context &io_context, const std::string &ip,
                   const std::string &port);
  void cleanUp();

  // update and draw functions
  void idleCallback();
  void displayCallback(GLFWwindow *);

  // callbacks - for interactions
  void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                   int mods);
  void mouseCallback(GLFWwindow *window, double xPos, double yPos);

  // Getters
  GLFWwindow *getWindow() { return window; }

private:
  // Camera properties
  std::unique_ptr<Camera> cam;
  float mouseX, mouseY;

  // Gamestate properties
  Cube *cube;
  Model *model;
  GameState *gameState;
  // Renderer* renderer;
  // Later: list <GameObject*> objects;

  // Shader Program
  unique_ptr<Shader> cubeShaderProgram;
  unique_ptr<Shader> modelShaderProgram;

  // Network
  std::unique_ptr<ClientNetwork> network;
};