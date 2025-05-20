#pragma once

#include "globals.hpp"

class CharacterManager {
public:
  CharacterManager();

  int chicken;
  int cow;
  int sheep;
  int pig;

  void setID(int id);

  void assignCharacter(Characters character, int clientID);

  void isAlreadyAssigned(int clientID);

  bool allAssigned();
};