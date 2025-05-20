#include "client_character_manager.hpp"

CharacterManager::CharacterManager() {
  chicken = -1;
  pig = -1;
  cow = -1;
  sheep = -1;
}

void CharacterManager::setID(int id) { whoAmI = id; }

void CharacterManager::setCharacter(int ch, int s, int p, int c) {
  struct CharacterSlot {
    int &current;
    int newVal;
    BaseUI *button;
    Characters characterType;
  };

  std::vector<CharacterSlot> slots = {
      {chicken, ch, UIManager::chickenButton.get(), Characters::CHICKEN},
      {sheep, s, UIManager::sheepButton.get(), Characters::SHEEP},
      {pig, p, UIManager::pigButton.get(), Characters::PIG},
      {cow, c, UIManager::cowButton.get(), Characters::COW}};

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
      cout << "Client " << whoAmI
             << " has been assigned to " << (uint8_t)slot.characterType << endl;
    }
    // Case 3: Character is assigned to someone else → lock the button
    else {
      UIManager::lockButton(slot.button);
    }

    // Update internal state
    slot.current = slot.newVal;
  }
}
