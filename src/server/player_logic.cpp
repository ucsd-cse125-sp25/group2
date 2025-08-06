#include "player_logic.hpp"

PlayerLogic::PlayerLogic() {
  speed = 35.0f;
  cowSpeed = 30.0f;
  chickenSpeed = 40.0f;
  sheepJumps = SHEEP_MAX_JUMPS;
  jumpForce = 60.0f;
  sheepJumpForce = 90.0f;
  glideFallSpeed = 10.0f;
  cowSpeed = 30.0f;
  chickenSpeed = 40.0f;

  for (int i = 0; i < NUM_PLAYERS; i++) {
    heldObjects[i] = nullptr;
    characterToClient[i] = -1;
    clientToCharacter[i] = -1;
  }
}

void PlayerLogic::move(GameObject *player, glm::vec3 direction) {
  // move the player
  auto rigidBody = player->getRigidBody();
  float spd = speed;
  if (player->getID() == CHICKEN) {
    spd = chickenSpeed;
  } else if (player->getID() == COW) {
    spd = cowSpeed;
  }
  rigidBody->applyImpulse(spd * direction);
}

OBJECT_ID PlayerLogic::moveHeldObject(PLAYER_ID id, GameObject *player) {
  // If the player is holding an object, move it with the player and return its
  // ID Otherwise, return -1
  if (getHeldObject(id) != nullptr) {
    auto heldObject = getHeldObject(id);
    auto tf = heldObject->getTransform();
    glm::vec3 offset = glm::vec3(0.0f, 4.0f, 0.0f);
    tf->setPosition(player->getTransform()->getPosition() + offset);
    return heldObject->getID();
  }
  return -1;
}

void PlayerLogic::jump(GameObject *player) {

  if (!player->isGrounded() && player->getID() != SHEEP) {
    return; // only sheep can jump in air
  }
  float force = jumpForce;
  if (player->getID() == SHEEP) {
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
    sheepDoubleJump = player->getID() == SHEEP ? false : sheepDoubleJump;
  } else if (player->getID() == SHEEP && !sheepDoubleJump) {
    sheepDoubleJump = true;
    auto rigidBody = player->getRigidBody();
    rigidBody->setVelocity(glm::vec3(rigidBody->getVelocity().x, 0.0f,
                                     rigidBody->getVelocity().z));
    rigidBody->setForce(glm::vec3(0.0f));
    rigidBody->applyImpulse(glm::vec3(0.0f, force, 0.0f));
  }
}

void PlayerLogic::rotate(GameObject *player, glm::vec3 rotation) {
  // rotate the player
  player->getTransform()->setRotation(rotation);
}

void PlayerLogic::reset(GameObject *player) {
  player->getTransform()->setPosition(player->getOriginalPosition());
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
  glm::vec3 offset = glm::vec3(0.0f, 4.0f, 0.0f);
  tf->setPosition(playerTransform->getPosition() + offset);
  object->setUsesGravity(false);
  object->getRigidBody()->setHeld(playerObject->getID());

  if (playerObject->getID() == PIG &&
      pigNotes.find(object->getID()) != pigNotes.end()) {
    pigNoteChange = true; // Pig has picked up a note
  }
}

void PlayerLogic::dropObject(GameObject *playerObject, GameObject *object) {
  auto tf = object->getTransform();
  glm::vec3 offset = playerObject->getTransform()->getForward() * 3.0f +
                     glm::vec3(0.0f, 2.0f, 0.0f);
  tf->setPosition(playerObject->getTransform()->getPosition() + offset);
  object->setUsesGravity(true);
  object->getRigidBody()->setHeld(-1);

  if (playerObject->getID() == PIG &&
      pigNotes.find(object->getID()) != pigNotes.end()) {
    pigNoteChange = true; // Pig has dropped a note
  }
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