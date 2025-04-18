#include "client/client.hpp"
#include <memory>

void error_callback(int error, const char* description) {
    // Print error.
    std::cerr << description << std::endl;
}

void setup_callbacks(GLFWwindow* window) {
    // Set the error callback.
    glfwSetErrorCallback(error_callback);

    /* Set key callback */
    glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        static_cast<Client*>(glfwGetWindowUserPointer(w))->keyCallback(w, key, scancode, action, mods);
    });

    /* Set mouse and cursor callbacks */
    glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int mods) {
        static_cast<Client*>(glfwGetWindowUserPointer(w))->mouse_callback(w, button, action, mods);
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double xposIn, double yposIn) {
        static_cast<Client*>(glfwGetWindowUserPointer(w))->cursor_callback(w, xposIn, yposIn);
    });
}

void setup_opengl_settings() {
    // Enable depth buffering.
    glEnable(GL_DEPTH_TEST);
    // Related to shaders and z value comparisons for the depth buffer.
    glDepthFunc(GL_LEQUAL);
    // Set polygon drawing mode to fill front and back of each polygon.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Set clear color to black.
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void print_versions() {
    // Get info of GPU and supported OpenGL version.
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version supported: " << glGetString(GL_VERSION)
              << std::endl;

    // If the shading language symbol is defined.
#ifdef GL_SHADING_LANGUAGE_VERSION
    std::cout << "Supported GLSL version is: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#endif
}

int main(void) {
    // Create the GLFW window.
    std::unique_ptr<Client> client(new Client());
    GLFWwindow* window = client->createWindow(800, 600);
    glfwSetWindowUserPointer(window, client.get());
    if (!window) exit(EXIT_FAILURE);

    print_versions();
    setup_callbacks(window);
    setup_opengl_settings();

    if (!client->initializeProgram()) exit(EXIT_FAILURE);

    asio::io_context io_context;
    if (!client->initializeNetwork(io_context, "127.0.0.1","12345")) exit(EXIT_FAILURE);

    while (!glfwWindowShouldClose(window)) {
        client->displayCallback(window);

        // Idle callback. Updating objects, etc. can be done here.
        client->idleCallback();
    }

    client->cleanUp();
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}