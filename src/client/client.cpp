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
#ifdef __APPLE__
  glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
#endif

  // Create the GLFW window
  window = glfwCreateWindow(windowWidth, windowHeight, "Barnyard Breakout",
                            NULL, NULL);
  if (!window) {
    cerr << "Window Creation Failed" << endl;
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(window);

  // Only initialize GLEW on non-Apple platforms
#ifndef __APPLE__
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    cerr << "GLEW initialization failed: " << glewGetErrorString(err) << endl;
    return false;
  }
#endif

  return true;
}

bool Client::initGameState() {
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

json Client::loadConfig(const string &path) {
  ifstream file(path);
  if (!file.is_open()) {
    throw runtime_error("Could not open config file at " + path);
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
    SoundManager::stopSound("TitleBGM");
    SoundManager::playSound("CharacterSelectBGM");
    UIManager::startScreenUI->isSelected = true;
  });
  UIManager::chickenButton->setOnClick([net = network.get()]() {
    CharacterSelectPacket packet(CHICKEN, net->getID());
    net->send(packet);
  });
  UIManager::pigButton->setOnClick([net = network.get()]() {
    CharacterSelectPacket packet(PIG, net->getID());
    net->send(packet);
  });
  UIManager::sheepButton->setOnClick([net = network.get()]() {
    CharacterSelectPacket packet(SHEEP, net->getID());
    net->send(packet);
  });
  UIManager::cowButton->setOnClick([net = network.get()]() {
    CharacterSelectPacket packet(COW, net->getID());
    net->send(packet);
  });
  UIManager::keypad->setOnInputCallback(
      [net = network.get()](OBJECT_ID id, int index) {
        KeypadInputPacket packet(id, net->getID(),
                                 UIManager::keypad->inputSequence, false);
        net->send(packet);
      });
  UIManager::keypad->setCloseCallback([net = network.get()](OBJECT_ID id) {
    KeypadInputPacket packet(id, net->getID(), UIManager::keypad->inputSequence,
                             true);
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
      cout << "Received INIT packet with ID: " << initPacket->id << endl;
      network->setID(initPacket->id);
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
        SoundManager::stopSound("CharacterSelectBGM");
        SoundManager::playSound("GameBGM");
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
      PLAYER_ID character = characterManager->selectedCharacter;
      game->setPlayer(character);
      cam->setRadius(cam->getCameraRadius(
          character)); // Set camera radius based on character
      break;
    }
    case PacketType::LEVELCHANGE: {
      auto levelPacket = dynamic_cast<LevelChangePacket *>(packet.get());
      game->changeLevel(levelPacket->level);
      break;
    }
    case PacketType::ACTIVATE: {
      auto activatePacket = dynamic_cast<ActivatePacket *>(packet.get());
      OBJECT_ID id = activatePacket->id;
      game->getObject(id)->activate();
      break;
    }
    case PacketType::DEACTIVATE: {
      auto deactivatePacket = dynamic_cast<DeactivatePacket *>(packet.get());
      OBJECT_ID id = deactivatePacket->id;
      game->getObject(id)->deactivate();
      break;
    }
    case PacketType::KEYPAD: {
      auto keypadPacket = dynamic_cast<KeypadPacket *>(packet.get());
      UIManager::keypad->setObjectID(keypadPacket->id);
      UIManager::keypad->display = keypadPacket->display;
      cout << "Keypad display: " << keypadPacket->display << endl;
      if (keypadPacket->display) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
      } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
      }
      UIManager::keypad->setUnlocked(keypadPacket->solved);
      break;
    }
    case PacketType::NOTE: {
      auto notePacket = dynamic_cast<NotePacket *>(packet.get());
      cout << "displaying note with ID: " << notePacket->id << endl;
      selectedNote = notePacket->id; // Store the note ID to display
      break;
    }
    }
  }

  UIManager::updateMenu(mouseX, mouseY, windowWidth, windowHeight, deltaTime,
                        game->state);

  if (game->state == Gamestate::GAME && !UIManager::keypad->display) {
    cam->update(xOffset, yOffset,
                game->getPlayer()->getPosition() +
                    cam->getCameraOffset(game->getPlayer()->getID()));
    xOffset = 0.0f;
    yOffset = 0.0f;
    updatePlayerRotation();
  }
}

void Client::displayCallback(GLFWwindow *window) {
  // Clear the color and depth buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  UIManager::drawMenu(game->state);

  // Draw objects
  if (game->state == Gamestate::GAME) {
    game->draw(cam->getViewProj(), cam->getPos());
  }

  if (selectedNote != -1) {
    game->displayNote(selectedNote);
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
    MovementPacket packet(game->getPlayer()->getID(), MovementType::FORWARD);
    network->send(packet);
    SoundManager::playSound("WalkingSound");
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    MovementPacket packet(game->getPlayer()->getID(), MovementType::BACKWARD);
    network->send(packet);
    SoundManager::playSound("WalkingSound");
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    MovementPacket packet(game->getPlayer()->getID(), MovementType::LEFT);
    network->send(packet);
    SoundManager::playSound("WalkingSound");
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    MovementPacket packet(game->getPlayer()->getID(), MovementType::RIGHT);
    network->send(packet);
    SoundManager::playSound("WalkingSound");
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    if (characterManager->selectedCharacter == CHICKEN) {
      MovementPacket packet(game->getPlayer()->getID(), MovementType::GLIDE);
      network->send(packet);
    }
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

    RotationPacket packet(game->getPlayer()->getID(), newRotation);
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
      MovementPacket packet(game->getPlayer()->getID(), MovementType::JUMP);
      network->send(packet);
      SoundManager::playSound("BounceSound");
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
  if (game->state != Gamestate::GAME || UIManager::keypad->display)
    return;
  if (action == GLFW_PRESS) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
      // Handle left mouse button press
      InteractionPacket packet(characterManager->selectedCharacter);
      network->send(packet);
    }
    // delete later: to switch between different clients on one machine
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
      static bool isCursorHidden = true;
      if (isCursorHidden) {
        // Show the cursor
        isCursorHidden = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
#ifndef __APPLE__
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
#endif
      } else {
        // Hide the cursor
        isCursorHidden = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#ifndef __APPLE__
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
#endif
      }
    }
  }
}