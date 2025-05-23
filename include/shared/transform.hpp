#pragma once

#include "core.hpp"

class Transform {
private:
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;

  glm::vec3 forward;
  glm::vec3 up;
  glm::vec3 right;

public:
  Transform(glm::vec3 pos = glm::vec3(0), glm::vec3 rot = glm::vec3(0),
            glm::vec3 scl = glm::vec3(1, 1, 1));

  glm::vec3 getPosition() const { return position; }
  glm::vec3 getRotation() const { return rotation; };
  glm::vec3 getScale() const { return scale; };
  glm::vec3 getForward() const { return forward; }
  glm::vec3 getUp() const { return up; };
  glm::vec3 getRight() const { return right; };

  void setPosition(glm::vec3 pos) { position = pos; }
  void setRotation(glm::vec3 rot) { rotation = rot; }
  void setScale(glm::vec3 scl) { scale = scl; }

  void updatePosition(glm::vec3 moveInput);
  void updateRotation(glm::vec3 rotationInput);
  void updateScale(float scaleInput);
};
