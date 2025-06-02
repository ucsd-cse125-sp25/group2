#pragma once

#include "globals.hpp"
#include "ui.hpp"

#include <array>
#include <vector>

#define CENTER_X 0.0f
#define CENTER_Y 0.0f

using namespace std;

class NoteUI {
private:
  OBJECT_ID id;

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

  bool unlocked;

public:
  NoteUI(OBJECT_ID objectId) : id(objectId) {};

  void draw();
  void update(float mouseX, float mouseY, int winWidth, int winHeight,
              float deltatime);

  void setOnInputCallback(function<void(OBJECT_ID id, int index)> callback);
  void setCloseCallback(function<void(OBJECT_ID id)> callback);

  OBJECT_ID getObjectID() const {
    return id;
  }

  bool display;
};