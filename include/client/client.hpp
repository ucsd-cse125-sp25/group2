#pragma once

#include "client/camera.hpp"
#include "client/clientnetwork.hpp"
#include "client/core.hpp"
#include "client/gamestate.hpp"
#include "client/model.hpp"
#include "client/shader.hpp"
#include "shared/objects/cube.hpp"

#define FPS (1.0 / 60.0)

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

  // creating the Window
  GLFWwindow *createWindow(int width, int height);

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
  // Later: list <GameObject*> objects;

  // Shader Program
  Shader cubeShaderProgram;
  Shader modelShaderProgram;

  // Network
  std::unique_ptr<ClientNetwork> network;
};