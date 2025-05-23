#pragma once

#include "ui.hpp"
#include <array>
#include <string>
#include <vector>

#define CENTER_X 0.0f
#define CENTER_Y 0.0f

class KeypadUI {
private:
  std::array<std::unique_ptr<BaseUI>, 4> buttons;
  std::vector<int> inputSequence;
  std::vector<int> correctSequence;
  std::function<void()> onUnlockCallback;

  float buttonSize = 0.2f;

  std::vector<std::unique_ptr<BaseUI>> shapeDisplays;
  std::array<GLuint, 4> buttonTextures;
  std::array<GLuint, 4> buttonHoverTextures;
  std::array<GLuint, 4> shapeTextures;

  void addInput(int index);
  void updateShapes();
  void loadTextures();
  void clearInput();

public:
  KeypadUI(const std::vector<int> &correctSequence);

  void draw();
  void update(float mouseX, float mouseY, int winWidth, int winHeight,
              float deltatime);

  void setOnUnlock(std::function<void()> callback);

  bool display;
  bool unlocked;
};