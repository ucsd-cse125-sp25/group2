#pragma once

#include "camera.hpp"
#include "client_gamestate.hpp"
#include "client_network.hpp"
#include "core.hpp"
#include "model.hpp"
#include "shader.hpp"
#include "ui_manager.hpp"

using namespace std;

class Client {
private:
  // Camera properties
  unique_ptr<Camera> cam;
  float mouseX, mouseY;

  // Gamestate properties
  unique_ptr<ClientGameState> game;

  // Network
  unique_ptr<ClientNetwork> network;

  // Key
  bool isHeldForward = false;  // W
  bool isHeldBackward = false; // S
  bool isHeldLeft = false;     // A
  bool isHeldRight = false;    // D

public:
  // Window properties
  GLFWwindow *window;
  int windowWidth;
  int windowHeight;

  // Constructors and desctructors
  Client();
  ~Client();

  bool init();
  bool initObjects();
  bool initNetwork(asio::io_context &io_context, const string &ip,
                   const string &port);
  bool initUI();

  void cleanUp();

  // update and draw functions
  void idleCallback();
  void displayCallback(GLFWwindow *);

  // process input
  void processInput(float deltaTime);

  // callbacks - for interactions
  void framebufferSizeCallback(GLFWwindow *window, int width, int height);
  void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                   int mods);
  void mouseCallback(GLFWwindow *window, double xPos, double yPos);
  void mouseButtonCallback(GLFWwindow *window, int button, int action,
                           int mods);

  // Getters
  GLFWwindow *getWindow() { return window; }

private:
  // Camera properties
  unique_ptr<Camera> cam;
  float mouseX, mouseY;

  // Gamestate properties
  unique_ptr<ClientGameState> game;

  unique_ptr<UIManager> ui;

  // Network
  unique_ptr<ClientNetwork> network;

  // Key
  bool isHeldForward = false;  // W
  bool isHeldBackward = false; // S
  bool isHeldLeft = false;     // A
  bool isHeldRight = false;    // D
};