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

bool Client::initUI() {
  ui = make_unique<UIManager>();
  ui->make_StartScreen();
  ui->setStartClick([&state = game->state]() { state = Gamestate::MAINMENU; });
  ui->make_mainMenu();
  ui->setMenuClick([&state = game->state]() {
    state = Gamestate::GAME;
    std::cout << "State changed to Playing!\n";
  });
  return true;
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

  cam->update(mouseX, mouseY, glm::vec3(0.0f, 0.0f, 0.0f));
}

void Client::displayCallback(GLFWwindow *window) {
  static double previousTime = glfwGetTime();

  double currentTime = glfwGetTime();
  float deltaTime = static_cast<float>(currentTime - previousTime);
  previousTime = currentTime;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Draw objects
  switch (game->state) {
  case Gamestate::STARTSCREEN:
    ui->draw_start();
    ui->update_start(mouseX, mouseY, windowWidth, windowHeight, deltaTime);
    break;
  case Gamestate::MAINMENU:
    ui->draw_menu();
    ui->update_menu(mouseX, mouseY, windowWidth, windowHeight, deltaTime);
    break;
  case Gamestate::GAME:
    game->draw(cam->getViewProj());
    break;
  default:
    cout << "Unhandled gamestate" << endl;
    break;
  }

  // Check events and swap buffers
  glfwPollEvents();

  // Main render display callback. Rendering of objects is done here.
  glfwSwapBuffers(window);
}

void Client::processInput(float deltaTime) {
  // Process WASD Movement
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    // cam->moveForward(deltaTime);
    MovementPacket packet(0, MovementType::FORWARD,
                          cam->getFacing()); // Hardcoded object ID for now
    // Later, we will use the ID of the player object
    network->send(packet);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    // cam->moveBackward(deltaTime);
    MovementPacket packet(0, MovementType::BACKWARD, cam->getFacing());
    network->send(packet);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    // cam->moveLeft(deltaTime);
    MovementPacket packet(0, MovementType::LEFT, cam->getFacing());
    network->send(packet);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    // cam->moveRight(deltaTime);
    MovementPacket packet(0, MovementType::RIGHT, cam->getFacing());
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

void Client::mouseButtonCallback(GLFWwindow *window, int button, int action,
                                 int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    // Handle left mouse button press
    InteractionPacket packet(0);
    network->send(packet);
    cout << "Left mouse button pressed" << endl;
  }
}