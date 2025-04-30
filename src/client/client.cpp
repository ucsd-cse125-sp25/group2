#include "client/client.hpp"

Client::Client() { cam = std::make_unique<Camera>(); }

Client::~Client() {}

bool Client::init() {
  // Initialize glfw
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return NULL;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // Enable forward compatibility and allow a modern OpenGL context
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Create the GLFW window.
  window = glfwCreateWindow(800, 600, "Barnyard Breakout", NULL, NULL);
  if (!window) {
    std::cerr << "Window Creation Failed" << std::endl;
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(window);
  glewInit();

  return true;
}

bool Client::initObjects() {
  // TODO:initialize the objects in the list of objects in the client game state
  cubeShaderProgram =
      make_unique<Shader>(Shader("../src/client/shaders/shader.vert",
                                 "../src/client/shaders/shader.frag"));
  cube = new Cube(glm::vec3(-20, -0.01, -20), glm::vec3(20, 0, 20),
                  glm::vec3(0, 0.95, 0));
  gameState = new GameState();
  return true;
}

bool Client::initNetwork(asio::io_context &io_context, const std::string &ip,
                         const std::string &port) {
  network = std::make_unique<ClientNetwork>(io_context, ip, port);
  return !network->err;
}

void Client::cleanUp() {
  delete cube;
  gameState->CleanUp();
  delete gameState;

  // Delete the shader programs
  // cubeShaderProgram.deleteShader();
  // modelShaderProgram.deleteShader();

  // Destroy GLFW window
  glfwDestroyWindow(window);
}

// Perform any updates to objects, camera, etc
void Client::idleCallback() {
  deque<std::unique_ptr<IPacket>> packets = network->receive();

  while (!packets.empty()) {
    std::unique_ptr<IPacket> packet = std::move(packets.front());
    packets.pop_front();

    switch (packet->get_type()) {
    case PacketType::INIT: {
      auto init_packet = dynamic_cast<InitPacket *>(packet.get());
      network->setId(init_packet->client_id);
      break;
    }
    case PacketType::POSITION: {
      auto position_packet = dynamic_cast<PositionPacket *>(packet.get());
      // initialize cube
      break;
    }
    case PacketType::OBJECT: {
      auto object_packet = dynamic_cast<ObjectPacket *>(packet.get());
      // initialize cube
      break;
    }
    }
  }

  cam->update(mouseX, mouseY);
  // I am passing a hardcoded value for deltaTime/frame rate. We can get the
  // real one from the network later
  gameState->Update(0.005f);
  // cube->update();
}

void Client::displayCallback(GLFWwindow *window) {
  // Clear the color and depth buffers.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Render the object.
  if (cube) {
    cube->draw(cam->getViewProj(), cubeShaderProgram);
  }

  std::vector<GameObject *> objects = gameState->getAllObjects();
  for (GameObject *o : objects)
    o->Render(cam->getViewProj());

  // Gets events, including input such as keyboard and mouse or window resizing.
  glfwPollEvents();

  // Main render display callback. Rendering of objects is done here.
  glfwSwapBuffers(window);
}

// callbacks - for Interaction
void Client::keyCallback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) {
  if (action == GLFW_PRESS) {
    if (key == GLFW_KEY_ESCAPE)
      glfwSetWindowShouldClose(window, true);
    gameState->keyStates[key] = true;
  }
  if (action == GLFW_RELEASE) {
    gameState->keyStates[key] = false;
  }
}

void Client::mouseCallback(GLFWwindow *window, double xPos, double yPos) {
  auto newMouseX = static_cast<float>(xPos);
  auto newMouseY = static_cast<float>(yPos);
  mouseX = newMouseX;
  mouseY = newMouseY;
}