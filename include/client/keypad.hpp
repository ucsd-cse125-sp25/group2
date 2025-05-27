#pragma once

#include "globals.hpp"
#include "ui.hpp"
#include <array>

#define CENTER_X 0.0f
#define CENTER_Y 0.0f

class KeypadUI {
private:
  std::array<std::unique_ptr<BaseUI>, 4> buttons;
  std::function<void(int index, OBJECT_ID id)> onInputCallback;

  float buttonSize = 0.25f;

  std::vector<std::unique_ptr<BaseUI>> shapeDisplays;
  std::array<GLuint, 4> buttonTextures;
  std::array<GLuint, 4> buttonHoverTextures;
  std::array<GLuint, 4> shapeTextures;

  void addInput(int index);
  void updateShapes();
  void loadTextures();
  void removeInputs();

  bool unlocked;

public:
  KeypadUI();

  void draw();
  void update(float mouseX, float mouseY, int winWidth, int winHeight,
              float deltatime);

  void setObjectID(OBJECT_ID objectId);

  void
  setOnInputCallback(std::function<void(OBJECT_ID id, int index)> callback);
  std::vector<int> inputSequence;

  void setUnlocked(bool isUnlocked);

  bool display;

  OBJECT_ID id;
};