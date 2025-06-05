#include "keypad.hpp"

KeypadUI::KeypadUI() : inputSequence(4) {
  loadTextures();
  solved = false;
  display = false;
  inputSequence.clear();

  float spacing = 0.35f;
  for (int i = 0; i < 4; ++i) {
    float x = CENTER_X + (i - 1.5f) * spacing;
    float y = CENTER_Y;
    buttons[i] =
        make_unique<BaseUI>(x, y, buttonSize, buttonSize, 1, true, true);
    buttons[i]->setTexture(buttonTextures[i]);
    buttons[i]->setHoverTexture(buttonHoverTextures[i]);

    int index = i;
    buttons[i]->setOnClick([this, index]() { addInput(index); });
    buttons[i]->setShader(
        make_unique<Shader>("../resources/shaders/animUi.vert",
                            "../resources/shaders/animUi.frag"));
  }

  buttons[4] = make_unique<BaseUI>(CENTER_X, CENTER_Y - 0.6f, buttonSize,
                                   buttonSize, 1, true, true);
  buttons[4]->setTexture(buttonTextures[4]);
  buttons[4]->setHoverTexture(buttonHoverTextures[4]);
  buttons[4]->setShader(make_unique<Shader>(
      "../resources/shaders/animUi.vert", "../resources/shaders/animUi.frag"));
  buttons[4]->setOnClick([this]() {
    onCloseCallback(id);
    display = false;
    inputSequence.clear();
    updateShapes();
  });

  for (int i = 0; i < 4; ++i) {
    float x = CENTER_X + (i - 1.5f) * spacing;
    float y = CENTER_Y + 0.4f;
    auto shape = make_unique<BaseUI>(x, y, 0.30f, 0.30f, 0);
    shape->setTexture(shapeTextures[4]);
    shape->setShader(make_unique<Shader>("../resources/shaders/animUi.vert",
                                         "../resources/shaders/animUi.frag"));
    shapeDisplays.push_back(move(shape));
  }
}

void KeypadUI::loadTextures() {
  buttonTextures = {BaseUI::loadTexture("../resources/ui/pin/sprite_00.png"),
                    BaseUI::loadTexture("../resources/ui/pin/sprite_02.png"),
                    BaseUI::loadTexture("../resources/ui/pin/sprite_04.png"),
                    BaseUI::loadTexture("../resources/ui/pin/sprite_06.png"),
                    BaseUI::loadTexture("../resources/ui/pin/sprite_08.png")};

  buttonHoverTextures = {
      BaseUI::loadTexture("../resources/ui/pin/sprite_01.png"),
      BaseUI::loadTexture("../resources/ui/pin/sprite_03.png"),
      BaseUI::loadTexture("../resources/ui/pin/sprite_05.png"),
      BaseUI::loadTexture("../resources/ui/pin/sprite_07.png"),
      BaseUI::loadTexture("../resources/ui/pin/sprite_09.png")};

  shapeTextures = {BaseUI::loadTexture("../resources/ui/pin/sprite_10.png"),
                   BaseUI::loadTexture("../resources/ui/pin/sprite_11.png"),
                   BaseUI::loadTexture("../resources/ui/pin/sprite_12.png"),
                   BaseUI::loadTexture("../resources/ui/pin/sprite_13.png"),
                   BaseUI::loadTexture("../resources/ui/pin/sprite_14.png")};
}

void KeypadUI::addInput(int index) {
  cout << "Keypad Input: " << index << endl;
  if (inputSequence.size() >= 4) {
    inputSequence.clear();
    updateShapes();
  }
  inputSequence.push_back(index);
  if (inputSequence.size() == 4) {
    if (onInputCallback) {
      cout << "sending input callback" << endl;
      onInputCallback(id, index);
    }
  }
}

void KeypadUI::updateShapes() {
  for (int i = 0; i < shapeDisplays.size(); ++i) {
    if (i < inputSequence.size()) {
      int shapeIDx = inputSequence[i];
      shapeDisplays[i]->setTexture(shapeTextures[shapeIDx]);
    } else {
      shapeDisplays[i]->setTexture(shapeTextures[4]);
    }
  }
}

void KeypadUI::draw() {
  for (auto &btn : buttons) {
    btn->draw();
  }
  for (auto &shape : shapeDisplays) {
    shape->draw();
  }
}

void KeypadUI::update(float mouseX, float mouseY, int winWidth, int winHeight,
                      float dt) {
  for (auto &btn : buttons) {
    btn->update(mouseX, mouseY, winWidth, winHeight, dt);
  }
  updateShapes();
}

void KeypadUI::setObjectID(OBJECT_ID objectID) { id = objectID; }

void KeypadUI::setOnInputCallback(
    function<void(int index, OBJECT_ID id)> callback) {
  onInputCallback = move(callback);
}

void KeypadUI::setUnlocked(bool isUnlocked) {
  if (!solved) {
    solved = isUnlocked;
  }
  // play animation or change UI state based on solved status
  else {
    inputSequence.clear();
  }
}
void KeypadUI::setCloseCallback(function<void(OBJECT_ID id)> callback) {
  onCloseCallback = move(callback);
}