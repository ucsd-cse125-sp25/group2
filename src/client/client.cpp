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
  cube = new Cube();
  return true;
}

bool Client::initNetwork(asio::io_context &io_context, const std::string &ip,
                         const std::string &port) {
  network = std::make_unique<ClientNetwork>(io_context, ip, port);
  return !network->err;
}

void Client::cleanUp() {
  delete cube;

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
  // cube->update();
}

void Client::displayCallback(GLFWwindow *window) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // test
  if (cube) {
    cube->draw(cam->getViewProj(), cubeShaderProgram);
  }

  glfwPollEvents();
  glfwSwapBuffers(window);
}

void Client::processInput(float deltaTime) {
  // Process WASD Movement
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    cam->moveForward(deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    cam->moveBackward(deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    cam->moveLeft(deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    cam->moveRight(deltaTime);
  }
}

// callbacks - for Interaction
void Client::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
  cam->updateAspect(width, height);
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