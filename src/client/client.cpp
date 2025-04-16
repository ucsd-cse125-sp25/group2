#include "client/client.hpp"

// Constructors and desctructors
bool Client::initializeProgram() {
    // Create a shader program with a vertex shader and a fragment shader.
    shaderProgram = LoadShaders("../src/client/shaders/shader.vert", "../src/client/shaders/shader.frag");

    // Check the shader program.
    if (!shaderProgram) {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }

    return true;
}

bool Client::initializeObjects() {
    // Create a cube
    cube = new Cube();
    // cube = new Cube(glm::vec3(-1, 0, -2), glm::vec3(1, 1, 1));

    return true;
}

bool Client::initializeNetwork(asio::io_context& io_context, const std::string& ip, const std::string& port) {
    network = new ClientNetwork(io_context, ip, port);
    return !network->err;
}

void Client::cleanUp() {
    delete network;
    // Deallcoate the objects.
    delete cube;

    // Delete the shader program.
    glDeleteProgram(shaderProgram);
}

// for the Window
GLFWwindow* Client::createWindow(int width, int height) {
    windowTitle = "Model Environment";
    // Initialize GLFW.
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return NULL;
    }

    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Ensure that minimum OpenGL version is 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Enable forward compatibility and allow a modern OpenGL context
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create the GLFW window.
    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

    // Check if the window could not be created.
    if (!window) {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return NULL;
    }

    // Make the context of the window.
    glfwMakeContextCurrent(window);

    // Set swap interval to 1.
    glewInit();
    glfwSwapInterval(0);

    // set up the camera
    cam = new Camera();
    cam->SetAspect(float(width) / float(height));

    // initialize the interaction var./iables
    leftDown = rightDown = false;
    mouseX = mouseY = 0;

    // Call the resize callback to make sure things get drawn immediately.
    Client::resizeCallback(window, width, height);

    return window;
}

void Client::resizeCallback(GLFWwindow* window, int width, int height) {
    this->width = width;
    this->height = height;
    // Set the viewport size.
    glViewport(0, 0, width, height);

    cam->SetAspect(float(width) / float(height));
}

// update and draw functions
void Client::idleCallback() {
    // Perform any updates as necessary.
    cam->Update();

    cube->update();
}

void Client::displayCallback(GLFWwindow* window) {

    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the object.
    cube->draw(cam->GetViewProjectMtx(), shaderProgram);

    // Gets events, including input such as keyboard and mouse or window resizing.
    glfwPollEvents();

    // Main render display callback. Rendering of objects is done here.
    glfwSwapBuffers(window);

}

// helper to reset the camera
void Client::resetCamera() {
    cam->Reset();
    cam->SetAspect(float(width) / float(height));
}

// callbacks - for Interaction
void Client::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    /*
     * TODO: Modify below to add your key callbacks.
     */

    // Check for a key press.
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;

            case GLFW_KEY_R:
                resetCamera();
                break;

            default:
                break;
        }
    }
}

void Client::mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        leftDown = (action == GLFW_PRESS);
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        rightDown = (action == GLFW_PRESS);
    }
}

void Client::cursor_callback(GLFWwindow* window, double currX, double currY) {
    int maxDelta = 100;
    int dx = glm::clamp((int)currX - mouseX, -maxDelta, maxDelta);
    int dy = glm::clamp(-((int)currY - mouseY), -maxDelta, maxDelta);

    mouseX = (int)currX;
    mouseY = (int)currY;

    // Move camera
    // NOTE: this should really be part of Camera::Update()
    if (leftDown) {
        const float rate = 1.0f;
        cam->SetAzimuth(cam->GetAzimuth() + dx * rate);
        cam->SetIncline(glm::clamp(cam->GetIncline() - dy * rate, -90.0f, 90.0f));
    }
    if (rightDown) {
        const float rate = 0.005f;
        float dist = glm::clamp(cam->GetDistance() * (1.0f - dx * rate), 0.01f, 1000.0f);
        cam->SetDistance(dist);
    }
}