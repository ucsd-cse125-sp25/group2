#include "keypad.hpp"

KeypadUI::KeypadUI() : inputSequence(4) {
  loadTextures();
  unlocked = false;
  display = false;
  inputSequence.clear();

  float spacing = 0.25f;
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

  for (int i = 0; i < 4; ++i) {
    float x = CENTER_X + (i - 1.5f) * spacing;
    float y = CENTER_Y + 0.4f;
    auto shape = make_unique<BaseUI>(x, y, 0.25f, 0.25f, 0);
    shape->setTexture(0);
    shape->setShader(make_unique<Shader>("../resources/shaders/animUi.vert",
                                         "../resources/shaders/animUi.frag"));
    shapeDisplays.push_back(move(shape));
  }
}

void KeypadUI::loadTextures() {
  buttonTextures = {
      BaseUI::loadTexture("../resources/ui/Buttons/sprite_00.png"),
      BaseUI::loadTexture("../resources/ui/Buttons/sprite_02.png"),
      BaseUI::loadTexture("../resources/ui/Buttons/sprite_04.png"),
      BaseUI::loadTexture("../resources/ui/Buttons/sprite_06.png")};
  buttonHoverTextures = {
      BaseUI::loadTexture("../resources/ui/Buttons/sprite_01.png"),
      BaseUI::loadTexture("../resources/ui/Buttons/sprite_03.png"),
      BaseUI::loadTexture("../resources/ui/Buttons/sprite_05.png"),
      BaseUI::loadTexture("../resources/ui/Buttons/sprite_07.png")};
  shapeTextures = {
      BaseUI::loadTexture("../resources/ui/Buttons/sprite_08.png"),
      BaseUI::loadTexture("../resources/ui/Buttons/sprite_09.png"),
      BaseUI::loadTexture("../resources/ui/Buttons/sprite_10.png"),
      BaseUI::loadTexture("../resources/ui/Buttons/sprite_11.png")};
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
      int shapeIdx = inputSequence[i];
      shapeDisplays[i]->setTexture(shapeTextures[shapeIdx]);
    } else {
      shapeDisplays[i]->setTexture(0);
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

void KeypadUI::setObjectID(OBJECT_ID objectId) { id = objectId; }

void KeypadUI::setOnInputCallback(
    function<void(int index, OBJECT_ID id)> callback) {
  onInputCallback = move(callback);
}

void KeypadUI::setUnlocked(bool isUnlocked) {
  unlocked = isUnlocked;
  // play animation or change UI state based on unlocked status
}