#pragma once

#include "core.hpp"

class Transform {
private:
  vec3 position;
  vec3 rotation;
  vec3 scale;

  vec3 forward;
  vec3 up;
  vec3 right;

public:
  Transform(vec3 pos = vec3(0), vec3 rot = vec3(0), vec3 scl = vec3(1, 1, 1));

  vec3 getPosition() const { return position; }
  vec3 getRotation() const { return rotation; };
  vec3 getScale() const { return scale; };
  vec3 getForward() const { return forward; }
  vec3 getUp() const { return up; };
  vec3 getRight() const { return right; };

  void setPosition(vec3 pos) { position = pos; }
  void setRotation(vec3 rot) { rotation = rot; }
  void setScale(vec3 scl) { scale = scl; }

  void updatePosition(vec3 moveInput);
  void updateRotation(vec3 rotationInput);
  void updateScale(float scaleInput);
};
