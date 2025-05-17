#include "ui_manager.hpp"

UIManager::UIManager() {}

void UIManager::make_StartScreen() {
  startScreenUI = make_unique<BaseUI>(0.0f, 0.0f, 2.0f, 2.0f, 0,
                                      AnimationInfo(3, 6, 0.06f), false, false);
  startScreenUI->setTexture(
      BaseUI::loadTexture("../resources/ui/TitleScreenAnim.png"));
  startScreenUI->setShader(make_unique<Shader>(
      "../resources/shaders/animUi.vert", "../resources/shaders/animUi.frag"));
  startButton = make_unique<BaseUI>(0.0f, -0.5f, 0.5f, 0.5f, 0,
                                    AnimationInfo(1, 2, 0.1f), true, true);
  startButton->setTexture(
      BaseUI::loadTexture("../resources/ui/StartButton.png"));
  startButton->setShader(make_unique<Shader>(
      "../resources/shaders/animUi.vert", "../resources/shaders/animUi.frag"));
  startButton->setHoverTexture(
      BaseUI::loadTexture("../resources/ui/StartButtonHover.png"));
}

void UIManager::update_start(float mouseX, float mouseY, int winWidth,
                             int winHeight, float deltatime) {
  startScreenUI->update(mouseX, mouseY, winWidth, winHeight, deltatime);
  startButton->update(mouseX, mouseY, winWidth, winHeight, deltatime);
}
void UIManager::update_menu(float mouseX, float mouseY, int winWidth,
                            int winHeight, float deltatime) {
  mainMenuUI->update(mouseX, mouseY, winWidth, winHeight, deltatime);
}
void UIManager::make_mainMenu() {
  mainMenuUI = make_unique<BaseUI>(0.0f, 0.0f, 2.0f, 2.0f, 0, true, false);
  mainMenuUI->setTexture(
      BaseUI::loadTexture("../resources/ui/CharacterSelection.png"));
  mainMenuUI->setShader(make_unique<Shader>(
      "../resources/shaders/baseUi.vert", "../resources/shaders/baseUi.frag"));
}
void UIManager::make_loadingScreen() {}

void UIManager::draw_start() {
  startScreenUI->draw();
  startButton->draw();
}
void UIManager::draw_menu() { mainMenuUI->draw(); }
void UIManager::draw_loading() {}

void UIManager::setStartClick(std::function<void()> callback) {
  startScreenUI->setOnClick(callback);
  startButton->setOnClick([this]() { startScreenUI->play(); });
}
void UIManager::setMenuClick(std::function<void()> callback) {
  mainMenuUI->setOnClick(callback);
}