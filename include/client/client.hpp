#pragma once

#include "camera.hpp"
#include "character_manager.hpp"
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
  float xOffset, yOffset;

  // Gamestate
  unique_ptr<ClientGameState> game;

  // Network
  unique_ptr<ClientNetwork> network;

  // Ui management / creation
  unique_ptr<UIManager> ui;

  // Character Selection
  unique_ptr<CharacterManager> characterManager;

  void updatePlayerRotation();

public:
  // Window properties
  GLFWwindow *window;
  int windowWidth;
  int windowHeight;

  // Constructors and desctructors
  Client();

  bool init();
  bool initObjects();
  bool initNetwork(asio::io_context &io_context);
  bool initUI();

  void cleanUp();

  // update and draw functions
  void idleCallback(float deltaTime);
  void displayCallback(GLFWwindow *window);

  // process wasd + jump input
  void processMovementInput();

  // callbacks - for interactions
  void framebufferSizeCallback(GLFWwindow *window, int width, int height);
  void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                   int mods);
  void mouseCallback(GLFWwindow *window, double xPos, double yPos);
  void mouseButtonCallback(GLFWwindow *window, int button, int action,
                           int mods);

  // Getters
  GLFWwindow *getWindow() { return window; }

  static json loadConfig(const std::string &path);
};