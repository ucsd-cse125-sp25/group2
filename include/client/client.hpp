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
  // Window Properties
  GLFWwindow *window;
  int width;
  int height;

  // list <GameObject*> objects;

  // Camera properties
  std::unique_ptr<Camera> cam;
  float mouseX, mouseY;

  // Objects to render
  Cube *cube;
  Model *model;
  GameState *gameState;

  // Shader Program
  Shader cubeShaderProgram;
  Shader modelShaderProgram;

  // Network
  ClientNetwork *network;

  // Constructors and desctructors
  Client();
  ~Client();

  bool init();
  bool initObjects();
  bool initNetwork(asio::io_context &io_context, const std::string &ip,
                   const std::string &port);
  void cleanUp();

  // for the Window
  GLFWwindow *createWindow(int width, int height);
  void resizeCallback(GLFWwindow *window, int width, int height);

  // update and draw functions
  void idleCallback();
  void displayCallback(GLFWwindow *);

  // callbacks - for interaction
  void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                   int mods);
  void mouseCallback(GLFWwindow *window, double xPos, double yPos);

  // Getters
  GLFWwindow *getWindow() { return window; }
};