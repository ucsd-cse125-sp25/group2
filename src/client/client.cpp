#include "client/client.hpp"

// Constructors and desctructors
bool Client::initializeProgram() {
    // Cube shader program
    cubeShaderProgram = Shader("../src/client/shaders/shader.vert", "../src/client/shaders/shader.frag"); 

    // Model shader program
    modelShaderProgram = Shader("../src/client/shaders/model.vert", "../src/client/shaders/model.frag"); 


    return true;
}

bool Client::initializeObjects() {
    // Create a cube
    cube = new Cube();
    // cube = new Cube(glm::vec3(-1, 0, -2), glm::vec3(1, 1, 1));

    // Load model
    model = new Model("../src/client/resources/objects/backpack/backpack.obj");

    return true;
}

bool Client::initializeCube(float x, float y, float z) {
    glm::vec3 center = glm::vec3(x,y,z);
    cube = new Cube(center - glm::vec3(1,1,1), center + glm::vec3(1,1,1));
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

    // Delete the shader programs.
    cubeShaderProgram.deleteShader();
    modelShaderProgram.deleteShader();

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
    deque<std::unique_ptr<IPacket>> packets = network->receive();
    std::unique_ptr<IPacket> first;
    if (packets.size() > 0) {
        packets.pop_front();
        first = std::move(packets.front());
    } else {
        first = nullptr;
    }

    if (first) {
        if (auto* posPacket = dynamic_cast<PositionPacket*>(first.get())) {
            initializeCube(posPacket->x, posPacket->y, posPacket->z);
        }
    }

    cam->Update();

    if (cube) cube->update();

    if (model) model->Update();
}

void Client::displayCallback(GLFWwindow* window) {

    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the object.
    if (cube) cube->draw(cam->GetViewProjectMtx(), cubeShaderProgram);

    // Render the model.
    if (model) model->Draw(cam->GetViewProjectMtx(), modelShaderProgram);

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
            case GLFW_KEY_W:
                network->send(ActionPacket(ActionType::FORWARD));
                break;
            case GLFW_KEY_A:
                network->send(ActionPacket(ActionType::LEFT));
                break;
            case GLFW_KEY_S:
                network->send(ActionPacket(ActionType::BACK));
                break;
            case GLFW_KEY_D:
                network->send(ActionPacket(ActionType::RIGHT));
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