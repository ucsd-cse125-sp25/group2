#include "client/client.hpp"
#include "client/clientnetwork.hpp"
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

// int main(void) {
//     // Create the GLFW window.
//     std::unique_ptr<Client> client(new Client());
//     // Client* client = new Client();
//     GLFWwindow* window = client->createWindow(800, 600);
//     glfwSetWindowUserPointer(window, client.get());
//     if (!window) exit(EXIT_FAILURE);

//     // Print OpenGL and GLSL versions.
//     print_versions();
//     // Setup callbacks.
//     setup_callbacks(window);
//     // Setup OpenGL settings.
//     setup_opengl_settings();

//     // Initialize the shader program; exit if initialization fails.
//     if (!client->initializeProgram()) exit(EXIT_FAILURE);
//     // Initialize objects/pointers for rendering; exit if initialization fails.
//     if (!client->initializeObjects()) exit(EXIT_FAILURE);

//     // Loop while GLFW window should stay open.
//     while (!glfwWindowShouldClose(window)) {
//         // Main render display callback. Rendering of objects is done here.
//         client->displayCallback(window);

//         // Idle callback. Updating objects, etc. can be done here.
//         client->idleCallback();
//     }

//     client->cleanUp();
//     // Destroy the window.
//     glfwDestroyWindow(window);
//     // Terminate GLFW.
//     glfwTerminate();

//     exit(EXIT_SUCCESS);
// }

int main() {
    try {
        asio::io_context io_context;

        ClientNetwork client(io_context, "127.0.0.1", "12345");

        std::string message = "Hello from Client!";
        std::cout << "Client sending message: " << message << "\n";
        client.send(message);

        std::string response = client.receive();
        std::cout << "Client received response: " << response << "\n";

    } catch (std::exception& e) {
        std::cerr << "Client exception: " << e.what() << "\n";
    }

    return 0;
}