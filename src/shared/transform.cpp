#include "shared/transform.hpp"
#include <iostream>

Transform::Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl) {
  this->position = pos;
  this->rotation = rot;
  this->scale = scl;
  this->forward = glm::vec3(1, 0, 0);
  this->up = glm::vec3(0, 1, 0);
  this->right = glm::vec3(0, 0, -1);
}

void Transform::updatePosition(glm::vec3 moveInput) { position += moveInput; }

void Transform::updateRotation(glm::vec3 rotateInput) {
  this->rotation += rotateInput;
  if (rotation.y > 360.0f)
    rotation.y -= 360.0f;
  if (rotation.y < 0.0f)
    rotation.y += 360.0f;

  float yaw = glm::radians(-this->rotation.y - 90);
  float pitch = glm::radians(this->rotation.x);

  glm::vec3 fwd;
  fwd.x = sin(yaw) * cos(pitch);
  fwd.y = sin(pitch);
  fwd.z = -cos(yaw) * cos(pitch);
  this->forward = glm::normalize(fwd);
  this->up = glm::vec3(0, 1, 0);
  this->right = glm::normalize(glm::cross(this->forward, this->up));

  // std::cout << forward.x << " " << forward.y << " " << forward.z <<
  // std::endl; std::cout << right.x << " " << right.y << " " << right.z <<
  // std::endl;
}

void Transform::updateScale(float scaleInput) {
  this->scale = glm::vec3(scaleInput);
}