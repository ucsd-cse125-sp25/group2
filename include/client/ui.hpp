#pragma once

#include "core.hpp"
#include <functional>
#include <iostream>
#include <stb_image.h>
#include "shader.hpp"
#include <memory>

class BaseUI {
public:
    int zIndex = 0;

    BaseUI(float x, float y, float width, float height, int zIndex, bool clickable = false, bool hoverable = false);

    virtual ~BaseUI();

    void setOnClick(std::function<void()> callback);

    void setTexture(GLuint texture);

    void setHoverTexture(GLuint texID);

    void setShader(unique_ptr<Shader> shader);

    void draw();

    void update(float mouseX, float mouseY, int winWidth, int winHeight);

    static GLuint loadTexture(const char* path);

    bool isHovered(float x_ndc, float y_ndc); 

private:
    // Position & size in NDC
    float x, y;
    float width, height;

    bool clickable;
    bool hoverable;

    unique_ptr<Shader> shader;

    // Textures
    GLuint textureID;
    GLuint hoverTextureID;  // Optional hover texture

    // Callbacks
    std::function<void()> onClickCallback;
    std::function<void()> onHoverEnterCallback;
    std::function<void()> onHoverExitCallback;

    // VAO, VBO, EBO
    GLuint VAO, VBO, EBO;

    bool hovered;

    void setupQuad();
};