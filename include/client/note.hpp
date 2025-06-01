#pragma once

#include "globals.hpp"
#include "ui.hpp"

#define CENTER_X 0.0f
#define CENTER_Y 0.0f

using namespace std;

class NoteUI {
private:
  function<void()> onCloseCallback;

  float buttonSize = 0.25f;

  unique_ptr<BaseUI> noteDisplay;
  unique_ptr<BaseUI> closeButton;

public:
  NoteUI();

  void draw();
  void update(float mouseX, float mouseY, int winWidth, int winHeight,
              float deltatime);

  void setObjectID(OBJECT_ID objectId);

  void setOnClose(function<void()> callback);

  bool display;

  OBJECT_ID id;
};