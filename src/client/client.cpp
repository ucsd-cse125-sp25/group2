#include "client.hpp"

Client::Client() {
  // Initialize window properties
  window = nullptr;
  windowWidth = WINDOW_WIDTH;
  windowHeight = WINDOW_HEIGHT;

  // Initialize camera properties
  cam = make_unique<Camera>();
  xOffset = 0.0f;
  yOffset = 0.0f;

  // Initialize game state properties
  game = make_unique<ClientGameState>();
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

  // Hide the cursor and lock it to the center of the window
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

  return true;
}

bool Client::initObjects() {
  if (!game->init()) {
    cerr << "ClientGameState Initialization Failed" << endl;
    return false;
  }
  return true;
}

bool Client::initNetwork(asio::io_context &io_context, const string &ip,
                         const string &port) {
  network = make_unique<ClientNetwork>(io_context, ip, port);
  return !network->err;
}

void Client::cleanUp() {
  // Destroy GLFW window
  glfwDestroyWindow(window);
}

// Perform any updates to objects, camera, etc
void Client::idleCallback() {
  deque<unique_ptr<IPacket>> packets = network->receive();

  while (!packets.empty()) {
    unique_ptr<IPacket> packet = move(packets.front());
    packets.pop_front();

    switch (packet->getType()) {
    case PacketType::INIT: {
      auto initPacket = dynamic_cast<InitPacket *>(packet.get());
      network->setId(initPacket->clientID);
      break;
    }
    case PacketType::OBJECT: {
      auto objectPacket = dynamic_cast<ObjectPacket *>(packet.get());
      game->update(objectPacket->objectID, &objectPacket->transform);
      break;
    }
    }
  }
  cam->update(xOffset, yOffset, game->getPlayer()->getPosition());
  xOffset = 0.0f;
  yOffset = 0.0f;
  updatePlayerRotation();
}

void Client::displayCallback(GLFWwindow *window) {
  // Clear the color and depth buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Draw objects
  game->draw(cam->getViewProj());

  // Main render display callback. Rendering of objects is done here
  glfwSwapBuffers(window);

  // Check events and swap buffers
  glfwPollEvents();
}

void Client::processMovementInput() {
  // Process WASD Movement
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    MovementPacket packet(game->getPlayer()->getId(), MovementType::FORWARD,
                          cam->getFacing());
    network->send(packet);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    MovementPacket packet(game->getPlayer()->getId(), MovementType::BACKWARD,
                          cam->getFacing());
    network->send(packet);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    MovementPacket packet(game->getPlayer()->getId(), MovementType::LEFT,
                          cam->getFacing());
    network->send(packet);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    MovementPacket packet(game->getPlayer()->getId(), MovementType::RIGHT,
                          cam->getFacing());
    network->send(packet);
  }
}

void Client::updatePlayerRotation() {
  float cameraYaw = cam->getRotation().y;
  float playerYaw = game->getPlayer()->getRotation().y;

  float targetYaw = -(cameraYaw - 90.0f);

  if (targetYaw > 360.0f || targetYaw < -360.0f)
    targetYaw = fmod(targetYaw, 360.0f);

  if (fabs(targetYaw - playerYaw) > 0.01f) {
    glm::vec3 currentRotation = game->getPlayer()->getRotation();
    glm::vec3 newRotation = glm::vec3(currentRotation.x, targetYaw,
                              currentRotation.z);
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
  }
}

void Client::mouseCallback(GLFWwindow *window, double xPos, double yPos) {
  static float lastX = 0.0f;
  static float lastY = 0.0f;
  static bool firstMouse = true;

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
  if (action == GLFW_PRESS) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
      // Handle left mouse button press
      InteractionPacket packet(0);
      network->send(packet);
      cout << "Left mouse button pressed" << endl;
    }
    // to switch between different clients on one machine
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