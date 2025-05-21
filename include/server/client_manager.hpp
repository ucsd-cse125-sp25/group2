#pragma once

#include "globals.hpp"

class ClientManager {
private:
  int characterToClient[4];
  Characters clientToCharacter[4];

public:
  ClientManager();

  void setID(int id);
  void assignCharacter(Characters character, int clientID);
  void unAssign(int clientID);

  bool allCharactersAssigned();

  int *getCharacterAssignments() { return characterToClient; };
  int getClient(Characters character) {
    return characterToClient[(int)character];
  };
  Characters getCharacter(int clientID) { return clientToCharacter[clientID]; };
};