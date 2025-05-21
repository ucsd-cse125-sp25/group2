#include "gamelogic.hpp"

GameLogic::GameLogic() {
  speed = 10.0f;
  jumpForce = 10.0f;

  // Initialize the held objects for each character
  heldObjects[Characters::CHICKEN] = -1;
  heldObjects[Characters::SHEEP] = -1;
  heldObjects[Characters::PIG] = -1;
  heldObjects[Characters::COW] = -1;
}

void GameLogic::moveObject(GameObject *object, glm::vec3 direction) {
  auto rigidBody = object->getRigidBody();
  if (rigidBody) {
    rigidBody->applyImpulse(speed * direction);
  }
}

void GameLogic::pickupObject(GameObject *playerObject, GameObject *object) {
  auto playerTransform = playerObject->getTransform();
  glm::vec3 offset = glm::vec3(0.0f, 5.0f, 0.0f);
  object->getTransform()->setPosition(playerTransform->getPosition() + offset);
}

void GameLogic::dropObject(GameObject *playerObject, GameObject *object) {
  auto playerTransform = playerObject->getTransform();
  object->getTransform()->setPosition(playerTransform->getPosition());
}