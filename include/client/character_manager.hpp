#pragma once

#include "globals.hpp"
#include "ui_manager.hpp"

#include <vector>

class CharacterManager {
public:
  CharacterManager();

  CLIENT_ID whoAmI;

  PLAYER_ID selectedCharacter;

  int chicken;
  int cow;
  int sheep;
  int pig;

  void setID(CLIENT_ID id);

  void setCharacter(PLAYER_ID ch, PLAYER_ID s, PLAYER_ID p, PLAYER_ID c);
};