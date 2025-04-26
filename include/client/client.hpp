#pragma once

#include "client/camera.hpp"
#include "client/clientnetwork.hpp"
#include "client/core.hpp"
#include "client/gamestate.hpp"
#include "client/model.hpp"
#include "client/shader.hpp"
#include "shared/cube.hpp"

#define FPS (1.0 / 60.0)

class Client {
public:
  // Window Properties
  int width;
  int height;
  const char *windowTitle;

  // list <GameObject*> objects;

  // Camera properties
  Camera *cam;
  bool leftDown, rightDown;
  int mouseX, mouseY;

  // Objects to render
  Cube *cube;
  Model *model;
  GameState *gameState;

  // Shader Program
  Shader cubeShaderProgram;
  Shader modelShaderProgram;

  // Network
  ClientNetwork *network;

  // Act as Constructors and desctructors
  bool initializeProgram();
  bool initializeObjects();
  bool initializeNetwork(asio::io_context &io_context, const std::string &ip,
                         const std::string &port);
  void cleanUp();

  // for the Window
  GLFWwindow *createWindow(int width, int height);
  void resizeCallback(GLFWwindow *window, int width, int height);

  // update and draw functions
  void idleCallback();
  void displayCallback(GLFWwindow *);

  // helper to reset the camera
  void resetCamera();

  // callbacks - for interaction
  void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                   int mods);
  void mouse_callback(GLFWwindow *window, int button, int action, int mods);
  void cursor_callback(GLFWwindow *window, double currX, double currY);

  // FOR TESTING
  bool initializeCube(glm::vec3 position);
};