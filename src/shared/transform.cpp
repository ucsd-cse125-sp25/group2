#include "transform.hpp"

Transform::Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl) {
  position = pos;
  rotation = rot;
  scale = scl;

  forward = glm::vec3(0, 0, -1);
  up = glm::vec3(0, 1, 0);
  right = glm::vec3(1, 0, 0);
}

void Transform::updatePosition(glm::vec3 moveInput) { position += moveInput; }

void Transform::updateRotation(glm::vec3 rotateInput) {
  rotation += rotateInput;

  // Normalize rotation angles to be within [0, 360)
  rotation.x = fmod(rotation.x, 360.0f);
  if (rotation.x < 0)
    rotation.x += 360.0f;

  rotation.y = fmod(rotation.y, 360.0f);
  if (rotation.y < 0)
    rotation.y += 360.0f;

  rotation.z = fmod(rotation.z, 360.0f);
  if (rotation.z < 0)
    rotation.z += 360.0f;

  float pitch = glm::radians(rotation.x);
  float yaw = glm::radians(rotation.y);
  float roll = glm::radians(rotation.z);

  // construct a rotation matrix from yaw, pitch, roll
  glm::mat4 rotMatrix = glm::yawPitchRoll(yaw, pitch, roll);

  // update forward, right, and up vectors
  forward = glm::normalize(glm::vec3(rotMatrix * glm::vec4(0, 0, -1, 0)));
  right = glm::normalize(glm::vec3(rotMatrix * glm::vec4(1, 0, 0, 0)));
  up = glm::normalize(glm::vec3(rotMatrix * glm::vec4(0, 1, 0, 0)));
}

void Transform::updateScale(float scaleInput) { scale *= scaleInput; }