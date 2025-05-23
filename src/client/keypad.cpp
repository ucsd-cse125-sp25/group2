#include "Keypad.hpp"

KeypadUI::KeypadUI(const std::vector<int> &correctSequence)
    : correctSequence(correctSequence) {
  loadTextures();
  unlocked = false;
  display = false;

  float spacing = 0.25f;
  for (int i = 0; i < 4; ++i) {
    float x = CENTER_X + (i - 1.5f) * spacing;
    float y = CENTER_Y;
    buttons[i] =
        std::make_unique<BaseUI>(x, y, buttonSize, buttonSize, 1, true, true);
    buttons[i]->setTexture(buttonTextures[i]);
    buttons[i]->setHoverTexture(buttonHoverTextures[i]);

    int index = i;
    buttons[i]->setOnClick([this, index]() { addInput(index); });
  }

  for (int i = 0; i < 4; ++i) {
    float x = CENTER_X + (i - 1.5f) * spacing;
    float y = CENTER_Y - 0.4f;
    auto shape = std::make_unique<BaseUI>(x, y, 0.15f, 0.15f, 0);
    shape->setTexture(0);
    shapeDisplays.push_back(std::move(shape));
  }
}

void KeypadUI::loadTextures() {
  buttonTextures = {
      BaseUI::loadTexture("../resources/ui/Buttons/sprite_00.png"),
      BaseUI::loadTexture("a../resources/ui/Buttons/sprite_02.png"),
      BaseUI::loadTexture("../resources/ui/Buttons/sprite_04.png"),
      BaseUI::loadTexture("a../resources/ui/Buttons/sprite_06.png")};
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
  if (inputSequence.size() >= correctSequence.size())
    return;

  inputSequence.push_back(index);
  updateShapes();

  if (inputSequence == correctSequence && onUnlockCallback) {
    onUnlockCallback();
    unlocked = true;
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
}

void KeypadUI::setOnUnlock(std::function<void()> callback) {
  onUnlockCallback = callback;
}
