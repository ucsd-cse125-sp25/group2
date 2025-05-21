#include "client_manager.hpp"

ClientManager::ClientManager() {
  for (int i = 0; i < 4; i++) {
    characterToClient[i] = -1;
    clientToCharacter[i] = Characters::NONE;
  }
}

void ClientManager::assignCharacter(Characters character, int clientID) {
  unAssign(clientID);
  if (characterToClient[(int)character] == -1) {
    characterToClient[(int)character] = clientID;
    clientToCharacter[clientID] = character;
  }
}

void ClientManager::unAssign(int clientID) {
  characterToClient[(int)clientToCharacter[clientID]] = -1;
  clientToCharacter[clientID] = Characters::NONE;
}

bool ClientManager::allCharactersAssigned() {
  for (int i = 0; i < 4; i++) {
    if (characterToClient[i] == -1) {
      return false;
    }
  }
  return true;
}