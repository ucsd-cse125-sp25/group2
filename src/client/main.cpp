#include "client.hpp"
#include <memory>
#include <stb_image.h>

using namespace std;

void error_callback(int error, const char *description) {
  // Print error
  cerr << description << endl;
}

void setup_callbacks(GLFWwindow *window) {
  // Set the error callback
  glfwSetErrorCallback(error_callback);

  /* Set framebuffer size callback */
  glfwSetFramebufferSizeCallback(
      window, [](GLFWwindow *w, int width, int height) {
        static_cast<Client *>(glfwGetWindowUserPointer(w))
            ->framebufferSizeCallback(w, width, height);
      });

  /* Set key callback */
  glfwSetKeyCallback(
      window, [](GLFWwindow *w, int key, int scancode, int action, int mods) {
        static_cast<Client *>(glfwGetWindowUserPointer(w))
            ->keyCallback(w, key, scancode, action, mods);
      });

  /* Set mouse and cursor callbacks */
  glfwSetCursorPosCallback(window,
                           [](GLFWwindow *w, double xposIn, double yposIn) {
                             static_cast<Client *>(glfwGetWindowUserPointer(w))
                                 ->mouseCallback(w, xposIn, yposIn);
                           });
}

void setup_opengl_settings() {
  // Textures are loaded in upside down, flip vertically
  stbi_set_flip_vertically_on_load(true);
  // Enable depth buffering
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  // Related to shaders and z value comparisons for the depth buffer
  glDepthFunc(GL_LEQUAL);
  // Set polygon drawing mode to fill front and back of each polygon
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  // Set clear color to black
  glClearColor(0, 0, 0, 0);
}

int main(void) {
  // Initialize client
  unique_ptr<Client> client(new Client());
  if (!client->init()) {
    cout << "Client Initialization Failed" << endl;
    exit(EXIT_FAILURE);
  }

  // Get window
  GLFWwindow *window = client->getWindow();
  if (!window)
    exit(EXIT_FAILURE);

  glfwSetWindowUserPointer(window, client.get());
  setup_callbacks(window);
  // Setup OpenGL settings
  setup_opengl_settings();

  // Create client network
  asio::io_context io_context;
  if (!client->initNetwork(io_context, "127.0.0.1", "12345")) {
    cout << "Client Network Initialization Failed" << endl;
    exit(EXIT_FAILURE);
  }

  // Delete later
  client->initObjects();

  float lastFrameTime = glfwGetTime();

  while (!glfwWindowShouldClose(window)) {
    // Calculate Frame Time
    float currentTime = glfwGetTime();
    float deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;

    client->processInput(deltaTime);

    // Updating of objects
    client->idleCallback();

    // Rendering call back
    client->displayCallback(window);
  }

  client->cleanUp();
  glfwTerminate();
  exit(EXIT_SUCCESS);
}