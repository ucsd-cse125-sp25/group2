#include "client_manager.hpp"

ClientManager::ClientManager() {
  for (int i = 0; i < 4; i++) {
    characterAssignments[i] = -1;
  }
}

void ClientManager::assignCharacter(Characters character, int clientID) {
  unAssign(clientID);
  if (characterAssignments[static_cast<int>(character)] == -1) {
    characterAssignments[static_cast<int>(character)] = clientID;
  }
}

void ClientManager::unAssign(int clientID) {
  for (int i = 0; i < 4; i++) {
    if (characterAssignments[i] == clientID) {
      characterAssignments[i] = -1;
      return;
    }
  }
}

bool ClientManager::allAssigned() {
  for (int i = 0; i < 4; i++) {
    if (characterAssignments[i] == -1) {
      return false;
    }
  }
  return true;
}