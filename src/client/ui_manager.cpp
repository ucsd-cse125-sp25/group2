#include "ui_manager.hpp"
#include <vector>

unique_ptr<BaseUI> UIManager::startScreenUI = nullptr;
unique_ptr<BaseUI> UIManager::startButton = nullptr;
unique_ptr<BaseUI> UIManager::mainMenuUI = nullptr;
unique_ptr<BaseUI> UIManager::loadingScreen = nullptr;
unique_ptr<BaseUI> UIManager::chickenButton = nullptr;
unique_ptr<BaseUI> UIManager::pigButton = nullptr;
unique_ptr<BaseUI> UIManager::sheepButton = nullptr;
unique_ptr<BaseUI> UIManager::cowButton = nullptr;
vector<BaseUI *> UIManager::characterButtons;

void UIManager::makeMenus() {
  // Start Screen + button
  startScreenUI = createUIElement(
      0.0f, 0.0f, 2.0f, 2.0f, 0, AnimationInfo(3, 6, 0.03f),
      "../resources/ui/TitleScreenAnim.png", nullptr, false, false);
  startButton =
      createUIElement(0.0f, -0.5f, 0.5f, 0.5f, 0, AnimationInfo(1, 3, 0.1f),
                      "../resources/ui/StartButton.png",
                      "../resources/ui/StartButtonHover.png", true, true);
  mainMenuUI = createUIElement(0.0f, 0.0f, 2.0f, 2.0f, 0, nullopt,
                               "../resources/ui/CharacterSelection.png",
                               nullptr, false, false);
  chickenButton =
      createUIElement(-0.6f, -0.8f, 0.4f, 0.4f, 0, AnimationInfo(1, 3, 0.1f),
                      "../resources/ui/ChickenButton.png",
                      "../resources/ui/ChickenButtonHover.png", true, true);
  sheepButton =
      createUIElement(0.2f, -0.8f, 0.4f, 0.4f, 0, AnimationInfo(1, 3, 0.1f),
                      "../resources/ui/SheepButton.png",
                      "../resources/ui/SheepButtonHover.png", true, true);
  pigButton =
      createUIElement(-0.2f, -0.8f, 0.4f, 0.4f, 0, AnimationInfo(1, 3, 0.1f),
                      "../resources/ui/PigButton.png",
                      "../resources/ui/PigButtonHover.png", true, true);
  cowButton =
      createUIElement(0.6f, -0.8f, 0.4f, 0.4f, 0, AnimationInfo(1, 3, 0.1f),
                      "../resources/ui/CowButton.png",
                      "../resources/ui/CowButtonHover.png", true, true);
  characterButtons = {chickenButton.get(), sheepButton.get(), pigButton.get(),
                      cowButton.get()};
}

unique_ptr<BaseUI> UIManager::createUIElement(
    float x, float y, float width, float height, int layer,
    std::optional<AnimationInfo> animInfo, const char *texturePath,
    const char *hoverTexturePath, bool isClickable, bool isHoverable) {
  unique_ptr<BaseUI> ui;

  if (animInfo.has_value()) {
    ui = make_unique<BaseUI>(x, y, width, height, layer, animInfo.value(),
                             isClickable, isHoverable);
  } else {
    ui = make_unique<BaseUI>(x, y, width, height, layer, isClickable,
                             isHoverable);
  }

  ui->setTexture(BaseUI::loadTexture(texturePath));

  if (hoverTexturePath != nullptr) {
    ui->setHoverTexture(BaseUI::loadTexture(hoverTexturePath));
  }

  ui->setShader(std::make_unique<Shader>("../resources/shaders/animUi.vert",
                                         "../resources/shaders/animUi.frag"));

  return ui;
}

void UIManager::updateMenu(float mouseX, float mouseY, int winWidth,
                            int winHeight, float deltatime, Gamestate state) {
  switch (state) {
  case Gamestate::STARTSCREEN:
    startScreenUI->update(mouseX, mouseY, winWidth, winHeight, deltatime);
    startButton->update(mouseX, mouseY, winWidth, winHeight, deltatime);
    break;
  case Gamestate::MAINMENU:
    for (auto *button : characterButtons) {
      button->update(mouseX, mouseY, winWidth, winHeight, deltatime);
    }
  default:
    break;
  }
}

void UIManager::drawMenu(Gamestate state) {
  switch (state) {
  case Gamestate::STARTSCREEN:
    startScreenUI->draw();
    startButton->draw();
    break;
  case Gamestate::MAINMENU:
    mainMenuUI->draw();
    chickenButton->draw();
    sheepButton->draw();
    pigButton->draw();
    cowButton->draw();
  default:
    break;
  }
}

void UIManager::deselectMenuButtons() {
  for (auto *button : characterButtons) {
    if (button->locked)
      continue;
    button->isClicked = false;
    button->isSelected = false;
    button->animInfo.currentFrame = 0;
    button->animInfo.startAnim = false;
  }
}

void UIManager::selectButton(BaseUI *button) {
  if (!button)
    return;
  button->isClicked = true;
  button->isSelected = true;
  button->animInfo.startAnim = true;
  button->animInfo.currentFrame =
      button->animInfo.rows * button->animInfo.cols - 1;
}

void UIManager::deselectButton(BaseUI *button) {
  if (!button)
    return;
  button->isClicked = false;
  button->isSelected = false;
  button->animInfo.currentFrame = 0;
  button->animInfo.startAnim = false;
}

void UIManager::lockButton(BaseUI *button) {
  if (!button)
    return;
  button->locked = true;
  selectButton(button);
}

void UIManager::unlockButton(BaseUI *button) {
  if (!button)
    return;
  button->locked = false;
  deselectButton(button);
}
