#pragma once

#include "core.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "shared/cube.hpp"
#include "client/clientnetwork.hpp"

#define FPS (1.0 / 60.0)

class Client {
public:
    // Window Properties
    int width;
    int height;
    const char* windowTitle;

    // Camera properties
    Camera* cam;
    bool leftDown, rightDown;
    int mouseX, mouseY;

    // Objects to render
    Cube* cube;

    // Shader Program
    GLuint shaderProgram;

    //Network
    ClientNetwork *network;

    // Act as Constructors and desctructors
    bool initializeProgram();
    bool initializeObjects();
    bool initializeNetwork(asio::io_context& io_context, const std::string& ip, const std::string& port);
    void cleanUp();

    // for the Window
    GLFWwindow* createWindow(int width, int height);
    void resizeCallback(GLFWwindow* window, int width, int height);

    // update and draw functions
    void idleCallback();
    void displayCallback(GLFWwindow*);

    // helper to reset the camera
    void resetCamera();

    // callbacks - for interaction
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void mouse_callback(GLFWwindow* window, int button, int action, int mods);
    void cursor_callback(GLFWwindow* window, double currX, double currY);
};