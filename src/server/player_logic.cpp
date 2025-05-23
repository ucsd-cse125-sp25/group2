#include "player_logic.hpp"

PlayerLogic::PlayerLogic() {
  speed = 10.0f;
  jumpForce = 10.0f;

  for (int i = 0; i < NUM_PLAYERS; i++) {
    heldObjects[i] = nullptr;
    characterToClient[i] = -1;
    clientToCharacter[i] = -1;
  }
}

vector<OBJECT_ID> PlayerLogic::move(PLAYER_ID id, GameObject *player,
                                    glm::vec3 direction) {
  vector<OBJECT_ID> movedObjects;

  // move the player
  auto rigidBody = player->getRigidBody();
  rigidBody->applyImpulse(speed * direction);
  movedObjects.push_back(id);

  // if the player is holding an object, move the object with the player
  OBJECT_ID heldObjectId = moveHeldObject(id, player);
  if (heldObjectId != -1) {
    movedObjects.push_back(heldObjectId);
  }

  return movedObjects;
}

vector<OBJECT_ID> PlayerLogic::rotate(PLAYER_ID id, GameObject *player,
                                      glm::vec3 rotation) {
  vector<OBJECT_ID> rotatedObjects;

  // rotate the player
  player->getTransform()->setRotation(rotation);

  // if the player is holding an object, apply the rotation to the object
  if (getHeldObject(id) != nullptr) {
    auto heldObject = getHeldObject(id);
    heldObject->getTransform()->setRotation(rotation);
    rotatedObjects.push_back(heldObject->getId());
  }
  return rotatedObjects;
}

// Returns the ID of the held object if it exists, otherwise -1
OBJECT_ID PlayerLogic::moveHeldObject(PLAYER_ID id, GameObject *player) {
  if (getHeldObject(id) != nullptr) {
    auto heldObject = getHeldObject(id);
    auto tf = heldObject->getTransform();
    glm::vec3 offset = glm::vec3(0.0f, 3.0f, 0.0f);
    tf->setPosition(player->getTransform()->getPosition() + offset);
    return heldObject->getId();
  }
  return -1;
}

void PlayerLogic::pickupObject(GameObject *playerObject, GameObject *object) {
  auto playerTransform = playerObject->getTransform();
  auto tf = object->getTransform();
  glm::vec3 offset = glm::vec3(0.0f, 3.0f, 0.0f);
  tf->setPosition(playerTransform->getPosition() + offset);
  object->setUsesGravity(false);
}

void PlayerLogic::dropObject(GameObject *playerObject, GameObject *object) {
  auto playerTransform = playerObject->getTransform();
  object->getTransform()->setPosition(playerTransform->getPosition());
  object->setUsesGravity(true);
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