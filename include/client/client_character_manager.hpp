#pragma once
#include "globals.hpp"
#include "ui_manager.hpp"
#include <vector>

class CharacterManager {
public:
  CharacterManager();

  int whoAmI;

  int chicken;
  int cow;
  int sheep;
  int pig;

  void setID(int id);

  void setCharacter(int ch, int s, int p, int c);
};