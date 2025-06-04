#pragma once

#include "core.hpp"
#include "shader.hpp"

#include <functional>
#include <iostream>
#include <memory>
#include <stb_image.h>

struct AnimationInfo {
  int cols, rows, currentFrame;
  float frameWidth, frameHeight, frameDuration, animationTimer;
  bool startAnim;
  AnimationInfo(int c, int r, float fD) : cols(c), rows(r), frameDuration(fD) {
    frameHeight = 1.0f / rows;
    frameWidth = 1.0f / cols;
    startAnim = false;
    currentFrame = 0;
    animationTimer = 0;
  }
  AnimationInfo()
      : cols(0), rows(0), frameWidth(1.0f), frameHeight(1.0f),
        frameDuration(0) {
    startAnim = false;
    currentFrame = 0;
    animationTimer = 0;
  }
};

class BaseUI {
private:
  // Position & size in NDC
  float x, y;
  float width, height;

  bool clickable;
  bool hoverable;

  bool isAnim;

  unique_ptr<Shader> shader;

  // Textures
  GLuint textureID;
  GLuint hoverTextureID;

  // Callbacks
  function<void()> onClickCallback;
  function<void()> onSelectCallback;

  // VAO, VBO, EBO
  GLuint VAO, VBO, EBO;

  bool hovered;

  void setupQuad();

public:
  int zIndex = 0;
  AnimationInfo animInfo;
  bool isClicked;
  bool isSelected;
  bool locked;

  BaseUI(float x, float y, float width, float height, int zIndex,
         bool clickable = false, bool hoverable = false);

  BaseUI(float x, float y, float width, float height, int zIndex,
         AnimationInfo animInfo, bool clickable = false,
         bool hoverable = false);

  virtual ~BaseUI();

  void setOnClick(function<void()> callback);

  void setOnSelect(function<void()> callback);

  void setTexture(GLuint texture);

  void setHoverTexture(GLuint texture);

  void setShader(unique_ptr<Shader> shader);

  void draw();

  void update(float mouseX, float mouseY, int winWidth, int winHeight,
              float deltatime);

  static GLuint loadTexture(const char *path);

  bool isHovered(float ndcX, float ndcY);

  void play();

  void resetAnim();
};