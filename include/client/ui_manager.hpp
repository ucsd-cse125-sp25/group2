#pragma once

#include "globals.hpp"
#include "ui.hpp"

#include <algorithm>
#include <optional>

class UIManager {
public:
  UIManager();
  static void make_menus();
  static void draw_menu(Gamestate state);

  static void update_menu(float mouseX, float mouseY, int winWidth,
                          int winHeight, float deltatime, Gamestate state);

  static unique_ptr<BaseUI>
  createUIElement(float x, float y, float width, float height, int layer,
                  std::optional<AnimationInfo> animInfo,
                  const char *texturePath,
                  const char *hoverTexturePath = nullptr,
                  bool isClickable = false, bool isHoverable = false);

  static void deselectMenuButtons();

  static void selectButton(BaseUI *button);
  static void deselectButton(BaseUI *button);

  static void lockButton(BaseUI *button);

  static void unlockButton(BaseUI *button);

  static unique_ptr<BaseUI> startScreenUI;
  static unique_ptr<BaseUI> startButton;

  static unique_ptr<BaseUI> mainMenuUI;
  static vector<BaseUI *> characterButtons;
  static unique_ptr<BaseUI> chickenButton;
  static unique_ptr<BaseUI> pigButton;
  static unique_ptr<BaseUI> sheepButton;
  static unique_ptr<BaseUI> cowButton;

  static unique_ptr<BaseUI> loadingScreen;
};