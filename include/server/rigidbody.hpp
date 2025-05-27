#pragma once

#include "core.hpp"

class RigidBody {
private:
  glm::vec3 velocity;
  glm::vec3 force;
  float mass;
  float restitution;
  float friction;
  float area;
  bool staticObject;

public:
  RigidBody(glm::vec3 v = glm::vec3(0), glm::vec3 f = glm::vec3(0), float m = 1)
      : velocity(v), force(f), mass(m) {
    friction = 0.5f;
    restitution = 0.1f;
    staticObject = true;
  }

  void setVelocity(glm::vec3 v) { velocity = v; }
  void setForce(glm::vec3 f) { force = f; }
  void setMass(float m) { mass = m; }
  void setArea(float a) { area = a; }
  void setStatic(bool isStatic) { staticObject = isStatic; }

  void applyForce(glm::vec3 f) { force += f; }
  void applyImpulse(glm::vec3 i) { velocity += i / mass; }

  glm::vec3 getVelocity() { return velocity; }
  glm::vec3 getForce() { return force; }
  float getMass() { return mass; }
  float getArea() { return area; }
  float getRestitution() { return restitution; }
  float getFriction() { return friction; }
  bool isStatic() { return staticObject; }
};