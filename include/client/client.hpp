#pragma once

#include "camera.hpp"
#include "cube.hpp"
#include "shader.hpp"
#include "core.hpp"

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

    // Act as Constructors and desctructors
    bool initializeProgram();
    bool initializeObjects();
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