#include "character_manager.hpp"

CharacterManager::CharacterManager() {
  chicken = -1;
  pig = -1;
  cow = -1;
  sheep = -1;
}

void CharacterManager::setID(CLIENT_ID id) { whoAmI = id; }

void CharacterManager::setCharacter(PLAYER_ID ch, PLAYER_ID s, PLAYER_ID p,
                                    PLAYER_ID c) {
  struct CharacterSlot {
    int &current;
    int newVal;
    BaseUI *button;
    int characterType;
  };

  std::vector<CharacterSlot> slots = {
      {chicken, ch, UIManager::chickenButton.get(), CHICKEN},
      {sheep, s, UIManager::sheepButton.get(), SHEEP},
      {pig, p, UIManager::pigButton.get(), PIG},
      {cow, c, UIManager::cowButton.get(), COW}};

  for (auto &slot : slots) {
    // Case 1: Character is now unassigned → unlock the button
    if (slot.newVal == -1) {
      UIManager::unlockButton(slot.button);
    }
    // Case 2: Character is assigned to this client → select the button
    else if (slot.newVal == whoAmI) {
      UIManager::deselectMenuButtons();
      UIManager::selectButton(slot.button);
      selectedCharacter = slot.characterType;
      cout << "Client " << whoAmI << " has been assigned to "
           << (uint8_t)slot.characterType << endl;
    }
    // Case 3: Character is assigned to someone else → lock the button
    else {
      UIManager::lockButton(slot.button);
    }

    // Update internal state
    slot.current = slot.newVal;
  }
}
