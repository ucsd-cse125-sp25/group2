#include "ui_manager.hpp"

UIManager::UIManager() {}

void UIManager::make_StartScreen() {
  startScreenUI = make_unique<BaseUI>(0.0f, 0.0f, 2.0f, 2.0f, 0, true, false);
  startScreenUI->setTexture(
      BaseUI::loadTexture("../resources/BarnyardBreakout.png"));
  startScreenUI->setShader(make_unique<Shader>(
      "../resources/shaders/baseUi.vert", "../resources/shaders/baseUi.frag"));
}
void UIManager::make_mainMenu() {}
void UIManager::make_loadingScreen() {}

void UIManager::draw_start() { startScreenUI->draw(); }
void UIManager::draw_menu() {}
void UIManager::draw_loading() {}

void UIManager::setStartClick(std::function<void()> callback) {
  startScreenUI->setOnClick(callback);
}