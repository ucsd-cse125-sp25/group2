#include "ui_manager.hpp"

unique_ptr<BaseUI> UIManager::startScreenUI = nullptr;
unique_ptr<BaseUI> UIManager::startButton = nullptr;
unique_ptr<BaseUI> UIManager::mainMenuUI = nullptr;
unique_ptr<BaseUI> UIManager::loadingScreen = nullptr;
unique_ptr<BaseUI> UIManager::gameCompletedScreen = nullptr;
unique_ptr<BaseUI> UIManager::chickenButton = nullptr;
unique_ptr<BaseUI> UIManager::pigButton = nullptr;
unique_ptr<BaseUI> UIManager::sheepButton = nullptr;
unique_ptr<BaseUI> UIManager::cowButton = nullptr;
unique_ptr<KeypadUI> UIManager::keypad = nullptr;
vector<BaseUI *> UIManager::characterButtons;

void UIManager::makeMenus() {
  // Start Screen + button
  startScreenUI = createUIElement(
      0.0f, 0.0f, 2.0f, 2.0f, 0, AnimationInfo(3, 6, 0.04f),
      "../resources/ui/screen/TitleScreenAnim.png", nullptr, false, false);
  startButton = createUIElement(
      0.0f, -0.5f, 0.5f, 0.5f, 0, AnimationInfo(1, 3, 0.1f),
      "../resources/ui/buttons/StartButton.png",
      "../resources/ui/buttons/StartButtonHover.png", true, true);
  mainMenuUI = createUIElement(0.0f, 0.0f, 2.0f, 2.0f, 0, nullopt,
                               "../resources/ui/screen/CharacterSelection.png",
                               nullptr, false, false);
  loadingScreen = createUIElement(
      0.0f, 0.0f, 2.0f, 2.0f, 0, AnimationInfo(6, 6, 0.1f),
      "../resources/ui/screen/LoadingScreen.png", nullptr, false, false);
  gameCompletedScreen = createUIElement(
      0.0f, 0.0f, 2.0f, 2.0f, 0, nullopt,
      "../resources/ui/screen/GameCompletedScreen.png", nullptr, false, false);
  chickenButton = createUIElement(
      -0.75f, -0.8f, 0.4f, 0.4f, 0, AnimationInfo(1, 3, 0.1f),
      "../resources/ui/buttons/ChickenButton.png",
      "../resources/ui/buttons/ChickenButtonHover.png", true, true);
  pigButton =
      createUIElement(-0.3f, -0.8f, 0.4f, 0.4f, 0, AnimationInfo(1, 3, 0.1f),
                      "../resources/ui/buttons/PigButton.png",
                      "../resources/ui/buttons/PigButtonHover.png", true, true);
  sheepButton = createUIElement(
      0.17f, -0.8f, 0.4f, 0.4f, 0, AnimationInfo(1, 3, 0.1f),
      "../resources/ui/buttons/SheepButton.png",
      "../resources/ui/buttons/SheepButtonHover.png", true, true);
  cowButton =
      createUIElement(0.65f, -0.8f, 0.4f, 0.4f, 0, AnimationInfo(1, 3, 0.1f),
                      "../resources/ui/buttons/CowButton.png",
                      "../resources/ui/buttons/CowButtonHover.png", true, true);
  characterButtons = {chickenButton.get(), pigButton.get(), sheepButton.get(),
                      cowButton.get()};
  keypad = make_unique<KeypadUI>();
}

unique_ptr<BaseUI> UIManager::createUIElement(
    float x, float y, float width, float height, int layer,
    optional<AnimationInfo> animInfo, const char *texturePath,
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

  ui->setShader(make_unique<Shader>("../resources/shaders/animUi.vert",
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
    break;
  case Gamestate::GAME:
    if (keypad && keypad->display) {
      keypad->update(mouseX, mouseY, winWidth, winHeight, deltatime);
    }
    break;
  case Gamestate::LOADING:
    if (loadingScreen) {
      loadingScreen->update(mouseX, mouseY, winWidth, winHeight, deltatime);
    }
    break;
  case Gamestate::COMPLETED:
    if (gameCompletedScreen) {
      gameCompletedScreen->update(mouseX, mouseY, winWidth, winHeight,
                                  deltatime);
    }
    break;
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
    pigButton->draw();
    sheepButton->draw();
    cowButton->draw();
    break;
  case Gamestate::GAME:
    if (keypad && keypad->display) {
      keypad->draw();
    }
    break;
  case Gamestate::LOADING:
    loadingScreen->draw();
    break;
  case Gamestate::COMPLETED:
    gameCompletedScreen->draw();
    break;
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
