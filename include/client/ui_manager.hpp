#pragma once
#include "ui.hpp"
#include <algorithm>

class UIManager {
public:
  UIManager();
  void make_StartScreen();
  void make_mainMenu();
  void make_loadingScreen();
  void draw_start();
  void draw_menu();
  void draw_loading();

  void setStartClick(std::function<void()> callback);
  void setMenuClick(std::function<void()> callback);

  void update_start(float mouseX, float mouseY, int winWidth, int winHeight,
                    float deltatime);
  void update_menu(float mouseX, float mouseY, int winWidth, int winHeight,
                   float deltatime);

  unique_ptr<BaseUI> startScreenUI;
  unique_ptr<BaseUI> startButton;
  std::shared_ptr<BaseUI> mainMenuUI;
  unique_ptr<BaseUI> loadingScreen;
};