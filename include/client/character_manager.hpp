#pragma once

#include "globals.hpp"
#include "ui_manager.hpp"

#include <vector>

class CharacterManager {
public:
  CharacterManager();

  CLIENT_ID whoAmI;

  PLAYER_ID selectedCharacter;

  void setID(CLIENT_ID id);

  void setCharacters(CLIENT_ID *characterAssignments);
};