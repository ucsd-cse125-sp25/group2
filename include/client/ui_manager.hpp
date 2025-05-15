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

  unique_ptr<BaseUI> startScreenUI;
  std::vector<std::shared_ptr<BaseUI>> mainMenuUI;
  unique_ptr<BaseUI> loadingScreen;
};