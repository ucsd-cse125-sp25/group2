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
  RigidBody(glm::vec3 v = glm::vec3(0), glm::vec3 f = glm::vec3(0),
            float m = 1) {
    this->velocity = v;
    this->force = f;
    this->mass = m;
    this->friction = 0.5f;
    this->restitution = 1.0f;
    this->staticObject = true;
  }

  void setVelocity(glm::vec3 v) { this->velocity = v; }
  void setForce(glm::vec3 f) { this->force = f; }
  void setMass(float m) { this->mass = m; }
  void setArea(float a) { this->area = a; }
  void setStatic(bool isStatic) { this->staticObject = isStatic; }

  void applyForce(glm::vec3 f) { this->force += f; }
  void applyImpulse(glm::vec3 i) { this->velocity += i / this->mass; }

  glm::vec3 getVelocity() { return this->velocity; }
  glm::vec3 getForce() { return this->force; }
  float getMass() { return this->mass; }
  float getArea() { return this->area; }
  float getRestitution() { return this->restitution; }
  float getFriction() { return this->friction; }
  bool isStatic() { return this->staticObject; }
};