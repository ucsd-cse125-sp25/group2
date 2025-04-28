#pragma once

#include "shared/core.hpp"

class Transform {
protected:
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;

public:
  Transform(glm::vec3 pos = glm::vec3(0), glm::vec3 rot = glm::vec3(0),
            glm::vec3 scl = glm::vec3(1, 1, 1));

  glm::vec3 getPosition() const {
    return this->position;
  }
  glm::vec3 getRotation() const {
    return this->rotation;
  };
  glm::vec3 getScale() const {
    return this->scale;
  };

  void setPosition(glm::vec3 pos) {
    this->position = pos;
  }
  void setRotation(glm::vec3 rot) {
    this->rotation = rot;
  }
  void setScale(glm::vec3 scl) {
    this->scale = scl;
  }

  void updatePosition(glm::vec3 moveInput);
  void updateRotation(glm::vec3 rotationInput);
  void updateScale(float scaleInput);
};
