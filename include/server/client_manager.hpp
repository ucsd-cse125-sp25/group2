#pragma once

#include "globals.hpp"

class ClientManager {
private:
  int characterAssignments[4];

  int characterHolding[4];

public:
  ClientManager();

  void setID(int id);

  void assignCharacter(Characters character, int clientID);

  void unAssign(int clientID);

  bool allAssigned();
};