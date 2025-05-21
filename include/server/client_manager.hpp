#pragma once

#include "globals.hpp"

class ClientManager {
private:
  int heldObjectsPerCharacter[4];
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
  int getHeldObject(Characters character) {
    return heldObjectsPerCharacter[(int)character];
  };
  Characters getCharacter(int clientID) { return clientToCharacter[clientID]; };
  int pickupObject(Characters character, int objectID) {
    int oldObject = heldObjectsPerCharacter[(int)character];
    heldObjectsPerCharacter[(int)character] = objectID;
    return oldObject;
  };
};