#include "server_character_manager.hpp"

CharacterManager::CharacterManager() {
  chicken = -1;
  pig = -1;
  cow = -1;
  sheep = -1;
}

void CharacterManager::assignCharacter(Characters character, int clientID) {
  isAlreadyAssigned(clientID);
  switch (character) {
  case Characters::CHICKEN:
    if (chicken == -1) {
      chicken = clientID;
    }
    break;
  case Characters::SHEEP:
    if (sheep == -1) {
      sheep = clientID;
    }
    break;
  case Characters::PIG:
    if (pig == -1) {
      pig = clientID;
    }
    break;
  case Characters::COW:
    if (cow == -1) {
      cow = clientID;
    }
    break;
  }
}

void CharacterManager::isAlreadyAssigned(int clientID) {
  if (chicken == clientID) {
    chicken = -1;
  }
  if (cow == clientID) {
    cow = -1;
  }
  if (sheep == clientID) {
    sheep = -1;
  }
  if (pig == clientID) {
    pig = -1;
  }
}

bool CharacterManager::allAssigned() {
  return (chicken != -1) && (sheep != -1) && (pig != -1) && (cow != -1);
}