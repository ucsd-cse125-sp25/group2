#pragma once

#include "globals.hpp"
#include "ui.hpp"

#include <array>
#include <vector>

#define CENTER_X 0.0f
#define CENTER_Y 0.0f

using namespace std;

class KeypadUI {
private:
  array<unique_ptr<BaseUI>, 5> buttons;
  function<void(int index, OBJECT_ID id)> onInputCallback;
  function<void(OBJECT_ID id)> onCloseCallback;

  float buttonSize = 0.25f;

  vector<unique_ptr<BaseUI>> shapeDisplays;
  array<GLuint, 5> buttonTextures;
  array<GLuint, 5> buttonHoverTextures;
  array<GLuint, 5> shapeTextures;

  void addInput(int index);
  void updateShapes();
  void loadTextures();
  void removeInputs();

  bool solved;

public:
  KeypadUI();

  void draw();
  void update(float mouseX, float mouseY, int winWidth, int winHeight,
              float deltatime);

  void setObjectID(OBJECT_ID objectID);

  void setOnInputCallback(function<void(OBJECT_ID id, int index)> callback);
  void setCloseCallback(function<void(OBJECT_ID id)> callback);

  vector<int> inputSequence;

  void setUnlocked(bool isUnlocked);

  bool display;

  OBJECT_ID id;
};