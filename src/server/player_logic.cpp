#include "player_logic.hpp"

PlayerLogic::PlayerLogic() {
  speed = 10.0f;
  jumpForce = 10.0f;

  for (int i = 0; i < NUM_PLAYERS; i++) {
    heldObjects[i] = -1;
    characterToClient[i] = -1;
    clientToCharacter[i] = -1;
  }
}

void PlayerLogic::move(PLAYER_ID id, GameObject *player, glm::vec3 direction) {
  auto rigidBody = player->getRigidBody();
  if (rigidBody) {
    rigidBody->applyImpulse(speed * direction);
  }
}

void PlayerLogic::pickupObject(GameObject *playerObject, GameObject *object) {
  auto playerTransform = playerObject->getTransform();
  glm::vec3 offset = glm::vec3(0.0f, 5.0f, 0.0f);
  object->getTransform()->setPosition(playerTransform->getPosition() + offset);
  object->setDisable(true);
}

void PlayerLogic::dropObject(GameObject *playerObject, GameObject *object) {
  auto playerTransform = playerObject->getTransform();
  object->getTransform()->setPosition(playerTransform->getPosition());
  object->setDisable(false);
}

void PlayerLogic::assignCharacter(PLAYER_ID playerID, CLIENT_ID clientID) {
  unAssignCharacter(clientID);
  if (characterToClient[playerID] == -1) {
    characterToClient[playerID] = clientID;
    clientToCharacter[clientID] = playerID;
  }
}

void PlayerLogic::unAssignCharacter(CLIENT_ID id) {
  if (clientToCharacter[id] == -1)
    return;
  characterToClient[clientToCharacter[id]] = -1;
  clientToCharacter[id] = -1;
}

bool PlayerLogic::allCharactersAssigned() {
  for (int i = 0; i < NUM_PLAYERS; i++) {
    if (characterToClient[i] == -1) {
      return false;
    }
  }
  return true;
}