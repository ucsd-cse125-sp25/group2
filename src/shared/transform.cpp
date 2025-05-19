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
  if (rotation.x > 360.0f || rotation.x < -360.0f)
    rotation.x = fmod(rotation.x, 360.0f);

  if (rotation.y > 360.0f || rotation.y < -360.0f)
    rotation.y = fmod(rotation.y, 360.0f);

  if (rotation.z > 360.0f || rotation.z < -360.0f)
    rotation.z = fmod(rotation.z, 360.0f);

  float yaw = rotation.y;
  float pitch = rotation.x;

  glm::vec3 fwd;
  fwd.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
  fwd.y = sin(glm::radians(pitch));
  fwd.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
  forward = glm::normalize(fwd);
  right = glm::normalize(glm::cross(forward, up));
}

void Transform::updateScale(float scaleInput) { scale = glm::vec3(scaleInput); }