#include "shared/transform.hpp"

Transform::Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl) {
  this->position = pos;
  this->rotation = rot;
  this->scale = scl;
}

glm::vec3 Transform::getPosition() { return this->position; }

glm::vec3 Transform::getRotation() { return this->rotation; }

glm::vec3 Transform::getScale() { return this->scale; }

void Transform::updatePosition(glm::vec3 moveInput) { position += moveInput; }

void Transform::updateRotation(glm::vec3 rotateInput) {
  this->rotation += rotateInput;
}

void Transform::updateScale(float scaleInput) {
  this->scale = glm::vec3(scaleInput);
}