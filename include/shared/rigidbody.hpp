#pragma once

#include "core.hpp"

class RigidBody {
private:
  glm::vec3 velocity;
  glm::vec3 force;
  float mass;
  float restitution;
  float area;

public:
  RigidBody(glm::vec3 v = glm::vec3(0), glm::vec3 f = glm::vec3(0),
            float m = 1) {
    this->velocity = v;
    this->force = f;
    this->mass = m;
    this->restitution = 1.0f;
  }

  void setVelocity(glm::vec3 v) { this->velocity = v; }
  void setForce(glm::vec3 f) { this->force = f; }
  void setMass(float m) { this->mass = m; }
  void setArea(float a) { this->area = a; }

  void applyForce(glm::vec3 f) { this->force += f; }
  void applyImpulse(glm::vec3 i) { this->velocity += i / this->mass; }

  glm::vec3 getVelocity() { return this->velocity; }
  glm::vec3 getForce() { return this->force; }
  float getMass() { return this->mass; }
  float getArea() { return this->area; }
  float getRestitution() { return this->restitution; }
};