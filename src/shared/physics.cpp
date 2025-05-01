#include "shared/physics.hpp"
#include <iostream>

void Physics::Add(GameObject *obj) { this->objects.push_back(obj); }

void Physics::Remove(GameObject *obj) {
  this->objects.erase(
      std::remove(this->objects.begin(), this->objects.end(), obj));
}

void Physics::Update(float deltaTime) {
  CalculateForces();
  ResolveCollisions(deltaTime);
  MoveObjects(deltaTime);
}

void Physics::CalculateForces() {
  for (GameObject *obj : this->objects) {
    glm::vec3 vel = obj->getRigidBody()->getVelocity();
    glm::vec3 force = obj->getRigidBody()->getMass() * this->gravity;
    if (vel != glm::vec3(0) && !obj->isGrounded())
      force += 0.5f * this->density * glm::dot(vel, vel) * this->drag *
               obj->getArea() * -1.0f * glm::normalize(vel);
    obj->getRigidBody()->applyForce(force);
  }
}

void Physics::ResolveCollisions(float deltaTime) {
  std::vector<Collision> collisions;
  for (GameObject *a : objects)
    for (GameObject *b : objects) {
      if (a == b)
        break;
      Collider *aBox = a->getCollider();
      Collider *bBox = b->getCollider();
      if (!aBox || !bBox)
        continue;

      if (aBox->intersect(bBox))
        collisions.emplace_back(Collision(a, b));
    }
  SolveCollisions(collisions, deltaTime);
}

void Physics::SolveCollisions(std::vector<Collision> collisions,
                              float deltaTime) {
  for (Collision c : collisions) {
    Solve(c, deltaTime);
  }
}

void Physics::Solve(Collision collision, float deltaTime) {
  GameObject *a = collision.a;
  GameObject *b = collision.b;
  RigidBody *a_rb = a->getRigidBody();
  RigidBody *b_rb = b->getRigidBody();
  glm::vec3 a_vel = a_rb->getVelocity();
  glm::vec3 b_vel = b_rb->getVelocity();
  // Normal doesn't make sense, might need to change collider completely for
  // this.
  glm::vec3 normal = glm::normalize(a->getTransform()->getPosition() -
                                    b->getTransform()->getPosition());

  float a_v_close = glm::dot(a_vel - b_vel, normal);
  float b_v_close = glm::dot(b_vel - a_vel, -normal);
  glm::vec3 impulse =
      -(1 + a_rb->getRestitution()) * a_rb->getMass() * a_v_close * normal;
  a_rb->applyImpulse(impulse);
  impulse =
      -(1 + a_rb->getRestitution()) * a_rb->getMass() * b_v_close * -normal;
  b_rb->applyImpulse(impulse);
}

void Physics::MoveObjects(float deltaTime) {
  for (GameObject *obj : this->objects) {
    Transform *tf = obj->getTransform();
    RigidBody *rb = obj->getRigidBody();
    Collider *cl = obj->getCollider();

    // Hardcoded Values, we should store these in animal class or a config file
    int id = obj->getId();
    float halfHeight = 1;
    if (id == 0)
      halfHeight = 0.25f;
    else if (id == 1)
      halfHeight = 0.5f;
    else if (id == 2)
      halfHeight = 0.625f;
    else if (id == 3)
      halfHeight = 0.75f;

    glm::vec3 vel =
        rb->getVelocity() + rb->getForce() / rb->getMass() * deltaTime;
    // Limit velocity to maxSpeed (Otherwise you go super fast mid air)
    glm::vec3 flatVel = glm::vec3(vel.x, 0, vel.z);
    float moveSpeed = 4;
    if (glm::length(flatVel) > moveSpeed) {
      glm::vec3 limitVel = moveSpeed * glm::normalize(flatVel);
      vel = glm::vec3(limitVel.x, vel.y, limitVel.z);
    }
    rb->setVelocity(vel);
    // std::cout << vel.x << ", " << vel.y << ", " << vel.z << std::endl;

    glm::vec3 pos = tf->getPosition() + rb->getVelocity() * deltaTime;
    if (pos.y <= halfHeight) {
      rb->setVelocity(glm::vec3(0));
      pos.y = halfHeight;
      obj->setGrounded(true);
    } else {
      obj->setGrounded(false);
    }

    tf->setPosition(pos);
    cl->updatePosition(pos);
    rb->setForce(glm::vec3(0));
  }
}