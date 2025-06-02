#include "player_logic.hpp"

PlayerLogic::PlayerLogic() {
  speed = 20.0f;
  sheepJumps = SHEEP_MAX_JUMPS;
  jumpForce = 40.0f;
  sheepJumpForce = 60.0f;
  glideFallSpeed = 1.0f;

  for (int i = 0; i < NUM_PLAYERS; i++) {
    heldObjects[i] = nullptr;
    characterToClient[i] = -1;
    clientToCharacter[i] = -1;
  }
}

void PlayerLogic::move(GameObject *player, glm::vec3 direction) {
  // move the player
  auto rigidBody = player->getRigidBody();
  rigidBody->applyImpulse(speed * direction);
}

OBJECT_ID PlayerLogic::moveHeldObject(PLAYER_ID id, GameObject *player) {
  // If the player is holding an object, move it with the player and return its
  // ID Otherwise, return -1
  if (getHeldObject(id) != nullptr) {
    auto heldObject = getHeldObject(id);
    auto tf = heldObject->getTransform();
    glm::vec3 offset = glm::vec3(0.0f, 2.0f, 0.0f);
    tf->setPosition(player->getTransform()->getPosition() + offset);
    return heldObject->getId();
  }
  return -1;
}

void PlayerLogic::jump(GameObject *player) {

  if (!player->isGrounded() && player->getId() != SHEEP) {
    return; // only sheep can jump in air
  }
  float force = jumpForce;
  if (player->getId() == SHEEP) {
    force = sheepJumpForce;
  }
  // jump the player
  if (player->isGrounded()) {
    player->setGrounded(false);
    auto rigidBody = player->getRigidBody();
    rigidBody->setVelocity(glm::vec3(rigidBody->getVelocity().x, 0.0f,
                                     rigidBody->getVelocity().z));
    rigidBody->setForce(glm::vec3(0.0f));
    rigidBody->applyImpulse(glm::vec3(0.0f, force, 0.0f));

    // If sheep is jumping from the ground, reset double jump
    sheepDoubleJump = player->getId() == SHEEP ? false : sheepDoubleJump;
  } else if (player->getId() == SHEEP && !sheepDoubleJump) {
    sheepDoubleJump = true;
    auto rigidBody = player->getRigidBody();
    rigidBody->setVelocity(glm::vec3(rigidBody->getVelocity().x, 0.0f,
                                     rigidBody->getVelocity().z));
    rigidBody->setForce(glm::vec3(0.0f));
    rigidBody->applyImpulse(glm::vec3(0.0f, force, 0.0f));
  }
}

vector<OBJECT_ID> PlayerLogic::rotate(PLAYER_ID id, GameObject *player,
                                      glm::vec3 rotation) {
  vector<OBJECT_ID> rotatedObjects;

  // rotate the player
  player->getTransform()->setRotation(rotation);
  rotatedObjects.push_back(id);

  // if the player is holding an object, apply the rotation to the object
  if (getHeldObject(id) != nullptr) {
    auto heldObject = getHeldObject(id);
    // heldObject->getTransform()->setRotation(rotation); comment this out for now
    rotatedObjects.push_back(heldObject->getId());
  }
  return rotatedObjects;
}

void PlayerLogic::glide(GameObject *chicken) {
  // glide the chicken
  if (!chicken->isGrounded()) {
    auto rigidBody = chicken->getRigidBody();
    if (rigidBody->getVelocity().y < 0) {
      rigidBody->setVelocity(glm::vec3(rigidBody->getVelocity().x,
                                       -glideFallSpeed,
                                       rigidBody->getVelocity().z));
      rigidBody->setForce(glm::vec3(0.0f));
    }
  }
}

void PlayerLogic::pickupObject(GameObject *playerObject, GameObject *object) {
  auto playerTransform = playerObject->getTransform();
  auto tf = object->getTransform();
  glm::vec3 offset = glm::vec3(0.0f, 2.0f, 0.0f);
  tf->setPosition(playerTransform->getPosition() + offset);
  object->setUsesGravity(false);
}

void PlayerLogic::dropObject(GameObject *playerObject, GameObject *object) {
  auto tf = object->getTransform();
  glm::vec3 offset = playerObject->getTransform()->getForward() * 3.0f +
                     glm::vec3(0.0f, 2.0f, 0.0f);
  tf->setPosition(playerObject->getTransform()->getPosition() + offset);
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