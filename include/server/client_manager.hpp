#pragma once

#include "globals.hpp"

class ClientManager {
private:
  int characterHolding[4];

public:
  int characterAssignments[4];
  ClientManager();

  void setID(int id);

  void assignCharacter(Characters character, int clientID);

  void unAssign(int clientID);

  bool allAssigned();
};