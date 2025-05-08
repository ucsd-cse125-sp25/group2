#include "server/gameobject.hpp"

void GameObject::applyMovement(const glm::vec3 &direction) {
  glm::vec3 pos = transform->getPosition();
  transform->setPosition(pos + direction * velocity);
}