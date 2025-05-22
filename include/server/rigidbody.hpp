#pragma once

#include "core.hpp"

class RigidBody {
private:
  vec3 velocity;
  vec3 force;
  float mass;
  float restitution;
  float friction;
  float area;
  bool staticObject;

public:
  RigidBody(vec3 v = vec3(0), vec3 f = vec3(0), float m = 1)
      : velocity(v), force(f), mass(m) {
    friction = 0.5f;
    restitution = 1.0f;
    staticObject = true;
  }

  void setVelocity(vec3 v) { velocity = v; }
  void setForce(vec3 f) { force = f; }
  void setMass(float m) { mass = m; }
  void setArea(float a) { area = a; }
  void setStatic(bool isStatic) { staticObject = isStatic; }

  void applyForce(vec3 f) { force += f; }
  void applyImpulse(vec3 i) { velocity += i / mass; }

  vec3 getVelocity() { return velocity; }
  vec3 getForce() { return force; }
  float getMass() { return mass; }
  float getArea() { return area; }
  float getRestitution() { return restitution; }
  float getFriction() { return friction; }
  bool isStatic() { return staticObject; }
};