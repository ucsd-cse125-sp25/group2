#pragma once

#include "core.hpp"

class RigidBody {
private:
  glm::vec3 velocity;
  glm::vec3 force;
  float mass;

public:
  RigidBody(glm::vec3 v = glm::vec3(0), glm::vec3 f = glm::vec3(0),
            float m = 1) {
    this->velocity = v;
    this->force = f;
    this->mass = m;
  }

  void setVelocity(glm::vec3 v) { this->velocity = v; }
  void setForce(glm::vec3 f) { this->force = f; }
  void setMass(float m) { this->mass = m; }

  void applyForce(glm::vec3 f) { this->force += f; }

  glm::vec3 getVelocity() { return this->velocity; }
  glm::vec3 getForce() { return this->force; }
  float getMass() { return this->mass; }
};