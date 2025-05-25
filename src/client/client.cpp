#include "client.hpp"

Client::Client() {
  // Initialize window properties
  window = nullptr;
  windowWidth = WINDOW_WIDTH;
  windowHeight = WINDOW_HEIGHT;

  // Initialize camera properties
  cam = make_unique<Camera>();
  mouseX = 0.0f;
  mouseY = 0.0f;
  xOffset = 0.0f;
  yOffset = 0.0f;

  // Initialize game state properties
  game = make_unique<ClientGameState>();
  characterManager = make_unique<CharacterManager>();
}

bool Client::init() {
  // Initialize glfw
  if (!glfwInit()) {
    cerr << "Failed to initialize GLFW" << endl;
    return false;
  }

  // Requests an OpenGL 3.3 context
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // Enable forward compatibility and allow a modern OpenGL context
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  // Window settings
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
  glfwWindowHint(GLFW_DECORATED, GL_TRUE);
  glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);

  // Create the GLFW window
  window = glfwCreateWindow(windowWidth, windowHeight, "Barnyard Breakout",
                            NULL, NULL);
  if (!window) {
    cerr << "Window Creation Failed" << endl;
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(window);

  GLenum err = glewInit();
  if (err != GLEW_OK) {
    cerr << "GLEW initialization failed: " << glewGetErrorString(err) << endl;
    return false;
  }

  return true;
}

bool Client::initObjects() {
  if (!game->init()) {
    cerr << "ClientGameState Initialization Failed" << endl;
    return false;
  }
  return true;
}

bool Client::initNetwork(asio::io_context &io_context) {
  network = make_unique<ClientNetwork>(io_context,
                                       loadConfig(CONFIG_PATH)["client-ip"],
                                       loadConfig(CONFIG_PATH)["port"]);
  return !network->err;
}

json Client::loadConfig(const std::string &path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open config file at " + path);
  }
  json j;
  file >> j;
  return j;
}

bool Client::initUI() {
  UIManager::makeMenus();
  UIManager::startButton->setOnClick(
      []() { UIManager::startScreenUI->play(); });
  UIManager::startButton->setOnSelect(
      []() { UIManager::startButton->isSelected = true; });
  UIManager::startScreenUI->setOnSelect([&state = game->state]() {
    state = Gamestate::MAINMENU;
    UIManager::startScreenUI->isSelected = true;
  });
  UIManager::chickenButton->setOnClick([net = network.get()]() {
    CharacterSelectPacket packet(CHICKEN, net->getId());
    net->send(packet);
  });
  UIManager::pigButton->setOnClick([net = network.get()]() {
    CharacterSelectPacket packet(PIG, net->getId());
    net->send(packet);
  });
  UIManager::sheepButton->setOnClick([net = network.get()]() {
    CharacterSelectPacket packet(SHEEP, net->getId());
    net->send(packet);
  });
  UIManager::cowButton->setOnClick([net = network.get()]() {
    CharacterSelectPacket packet(COW, net->getId());
    net->send(packet);
  });
  return true;
}

void Client::cleanUp() {
  // Destroy GLFW window
  glfwDestroyWindow(window);
}

// Perform any updates to objects, camera, etc
void Client::idleCallback(float deltaTime) {
  deque<unique_ptr<IPacket>> packets = network->receive();

  while (!packets.empty()) {
    unique_ptr<IPacket> packet = move(packets.front());
    packets.pop_front();

    switch (packet->getType()) {
    case PacketType::INIT: {
      auto initPacket = dynamic_cast<InitPacket *>(packet.get());
      network->setId(initPacket->id);
      characterManager->setID(initPacket->id);
      break;
    }
    case PacketType::OBJECT: {
      auto objectPacket = dynamic_cast<ObjectPacket *>(packet.get());
      game->update(objectPacket->id, &objectPacket->transform);
      break;
    }
    case PacketType::GAMESTATE: {
      auto statePacket = dynamic_cast<GameStatePacket *>(packet.get());
      game->state = statePacket->state;
      if (game->state == Gamestate::GAME) {
        // Hide the cursor and lock it to the center of the window when the game
        // starts
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#if !defined(__APPLE__)
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
#endif
      }
      break;
    }
    case PacketType::CHARACTERRESPONSE: {
      auto characterPacket =
          dynamic_cast<CharacterResponsePacket *>(packet.get());
      characterManager->setCharacters(characterPacket->characterAssignments);
      break;
    }
    }
  }
  if (game->state == Gamestate::STARTSCREEN ||
      game->state == Gamestate::MAINMENU) {
    UIManager::updateMenu(mouseX, mouseY, windowWidth, windowHeight, deltaTime,
                          game->state);
  }

  if (game->state == Gamestate::GAME) {
    cout << "Player position " << game->getPlayer()->getPosition().x << ", "
         << game->getPlayer()->getPosition().y << ", "
         << game->getPlayer()->getPosition().z << endl;
    cam->update(xOffset, yOffset, game->getPlayer()->getPosition());
    xOffset = 0.0f;
    yOffset = 0.0f;
    updatePlayerRotation();
  }
}

void Client::displayCallback(GLFWwindow *window) {
  // Clear the color and depth buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (game->state == Gamestate::STARTSCREEN ||
      game->state == Gamestate::MAINMENU) {
    UIManager::drawMenu(game->state);
  }

  // Draw objects
  if (game->state == Gamestate::GAME) {
    game->draw(cam->getViewProj());
  }

  // Main render display callback. Rendering of objects is done here
  glfwSwapBuffers(window);

  // Check events and swap buffers
  glfwPollEvents();
}

void Client::processMovementInput() {
  // Process WASD Movement
  if (game->state != Gamestate::GAME)
    return;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    MovementPacket packet(game->getPlayer()->getId(), MovementType::FORWARD);
    network->send(packet);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    MovementPacket packet(game->getPlayer()->getId(), MovementType::BACKWARD);
    network->send(packet);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    MovementPacket packet(game->getPlayer()->getId(), MovementType::LEFT);
    network->send(packet);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    MovementPacket packet(game->getPlayer()->getId(), MovementType::RIGHT);
    network->send(packet);
  }
}

void Client::updatePlayerRotation() {
  float cameraYaw = cam->getRotation().y;
  float playerYaw = game->getPlayer()->getRotation().y;

  float targetYaw = -(cameraYaw - 90.0f);

  targetYaw = fmod(targetYaw, 360.0f);
  if (targetYaw < 0.0f)
    targetYaw += 360.0f;

  float angleDiff = targetYaw - playerYaw;
  if (angleDiff > 180.0f)
    angleDiff -= 360.0f;
  if (angleDiff < -180.0f)
    angleDiff += 360.0f;

  if (fabs(angleDiff) > 0.01f) {
    glm::vec3 currentRotation = game->getPlayer()->getRotation();
    glm::vec3 newRotation =
        glm::vec3(currentRotation.x, targetYaw, currentRotation.z);
    game->getPlayer()->getTransform()->setRotation(newRotation);

    RotationPacket packet(game->getPlayer()->getId(), newRotation);
    network->send(packet);
  }
}

// callbacks - for Interaction
void Client::framebufferSizeCallback(GLFWwindow *window, int width,
                                     int height) {
  windowWidth = width;
  windowHeight = height;
  glViewport(0, 0, windowWidth, windowHeight);
  cam->updateAspect(windowWidth, windowHeight);
}

void Client::keyCallback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) {
  if (action == GLFW_PRESS) {
    if (key == GLFW_KEY_ESCAPE)
      glfwSetWindowShouldClose(window, true);
    if (key == GLFW_KEY_SPACE) {
      MovementPacket packet(game->getPlayer()->getId(), MovementType::JUMP,
                            vec3(0.0f));
      network->send(packet);
    }
  }
}

void Client::mouseCallback(GLFWwindow *window, double xPos, double yPos) {
  static float lastX = 0.0f;
  static float lastY = 0.0f;
  static bool firstMouse = true;

  mouseX = static_cast<float>(xPos);
  mouseY = static_cast<float>(yPos);

  if (firstMouse) {
    lastX = static_cast<float>(xPos);
    lastY = static_cast<float>(yPos);
    firstMouse = false;
  }

  xOffset = static_cast<float>(xPos) - lastX;
  yOffset = lastY - static_cast<float>(yPos);

  lastX = static_cast<float>(xPos);
  lastY = static_cast<float>(yPos);
}

void Client::mouseButtonCallback(GLFWwindow *window, int button, int action,
                                 int mods) {
  if (game->state != Gamestate::GAME)
    return;
  if (action == GLFW_PRESS) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
      // Handle left mouse button press
      glm::vec3 rayOrigin = cam->getPos();
      glm::vec3 rayDirection = cam->getFacing();
      InteractionPacket packet(characterManager->selectedCharacter,
                               rayDirection, rayOrigin);
      network->send(packet);
    }
    // delete later: to switch between different clients on one machine
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
      static bool isCursorHidden = true;
      if (isCursorHidden) {
        // Show the cursor
        isCursorHidden = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
      } else {
        // Hide the cursor
        isCursorHidden = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
      }
    }
  }
}