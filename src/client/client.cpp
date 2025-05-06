#include "client/client.hpp"

Client::Client() {
  // Initialize window properties
  window = nullptr;
  windowWidth = WINDOW_WIDTH;
  windowHeight = WINDOW_HEIGHT;

  // Initialize camera properties
  cam = make_unique<Camera>(
      glm::vec3(0.0f, 0.0f, 0.0f)); // Hardcoded target for now
  mouseX = 0.0f;
  mouseY = 0.0f;

  // Initialize game state properties
  game = make_unique<ClientGameState>();
}

Client::~Client() {}

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
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  // Window settings
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
  glfwWindowHint(GLFW_DECORATED, GL_TRUE);
  // glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE); Enable later

  // Create the GLFW window
  window = glfwCreateWindow(windowWidth, windowWidth, "Barnyard Breakout", NULL,
                            NULL);
  if (!window) {
    cerr << "Window Creation Failed" << endl;
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(window);

  glewInit();

  return true;
}

bool Client::initObjects() {
  if (!game->init()) {
    cerr << "GameState Initialization Failed" << endl;
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
      auto init_packet = dynamic_cast<InitPacket *>(packet.get());
      network->setId(init_packet->clientID);
      break;
    }
    case PacketType::OBJECT: {
      auto object_packet = dynamic_cast<ObjectPacket *>(packet.get());

      // call gameState->update() here w/ id and transform
      break;
    }
    }
  }

  cam->update(
      mouseX, mouseY,
      glm::vec3(0.0f, 0.0f, 0.0f)); // Later: gamestate->getPlayerPosition()
  game->update();
}

void Client::displayCallback(GLFWwindow *window) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Draw objects
  game->draw(cam->getViewProj());

  // Check events and swap buffers
  glfwPollEvents();
  glfwSwapBuffers(window);
}

void Client::processInput(float deltaTime) {
  // Process WASD Movement
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    cam->moveForward(deltaTime);
    MovementPacket packet(0,
                          MovementType::FORWARD); // Hardcoded object ID for now
    // Later, we will use the ID of the player object
    network->send(packet);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    cam->moveBackward(deltaTime);
    MovementPacket packet(0, MovementType::BACKWARD);
    network->send(packet);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    cam->moveLeft(deltaTime);
    MovementPacket packet(0, MovementType::LEFT);
    network->send(packet);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    cam->moveRight(deltaTime);
    MovementPacket packet(0, MovementType::RIGHT);
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
  auto newMouseX = static_cast<float>(xPos);
  auto newMouseY = static_cast<float>(yPos);
  mouseX = newMouseX;
  mouseY = newMouseY;
}