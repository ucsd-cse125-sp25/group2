#include "character_manager.hpp"

CharacterManager::CharacterManager() { selectedCharacter = -1; }

void CharacterManager::setID(CLIENT_ID id) { whoAmI = id; }

void CharacterManager::setCharacters(CLIENT_ID *characterAssignments) {
  for (int i = 0; i < NUM_PLAYERS; i++) {
    // Case 1: Character is now unassigned → unlock the button
    if (characterAssignments[i] == -1) {
      UIManager::unlockButton(UIManager::characterButtons[i]);
      // Case 2: Character is assigned to this client → select the button
    } else if (characterAssignments[i] == whoAmI) {
      UIManager::deselectMenuButtons();
      UIManager::selectButton(UIManager::characterButtons[i]);
      selectedCharacter = i;
      switch (i) {
        case 0:
          SoundManager::playSound("ChickenSound");
          break;
        case 1:
          SoundManager::playSound("PigSound");
          break;
        case 2:
          SoundManager::playSound("SheepSound");
          break;
        case 3:
          SoundManager::playSound("CowSound");
          break;
      }
      // Case 3: Character is assigned to someone else → lock the button
    } else {
      UIManager::lockButton(UIManager::characterButtons[i]);
    }
  }
}
