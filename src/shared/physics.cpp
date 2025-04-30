#include "shared/physics.hpp"
#include <iostream>

void Physics::Add(GameObject *obj) { this->objects.push_back(obj); }

void Physics::Remove(GameObject *obj) {
  this->objects.erase(
      std::remove(this->objects.begin(), this->objects.end(), obj));
}

void Physics::Update(float deltaTime) {
  ResolveCollisions(deltaTime);
  for (GameObject *obj : this->objects) {
    Transform *tf = obj->getTransform();
    RigidBody *rb = obj->getRigidBody();
    Collider *cl = obj->getCollider();

        // Hardcoded Values, we should store these in animal class maybe or a config file
        float halfHeight = 1;
        if (tf->getScale().x == 1.25f) halfHeight = 2.0f;
        else if (tf->getScale().x == 1.0f) halfHeight = 0.1;
        else if (tf->getScale().x == 0.75f) halfHeight = 1.2f;
        else if (tf->getScale().x == 0.5f) halfHeight = 1.025f;
        
        rb->applyForce(rb->getMass() * this->gravity);

    glm::vec3 vel =
        rb->getVelocity() + rb->getForce() / rb->getMass() * deltaTime;
    if (tf->getPosition().y <= halfHeight)
      vel = glm::vec3(0);
    rb->setVelocity(vel);

    glm::vec3 pos = tf->getPosition() + rb->getVelocity() * deltaTime;
    if (pos.y < halfHeight)
      pos.y = halfHeight;
    tf->setPosition(pos);
    cl->updatePosition(pos);
    rb->setForce(glm::vec3(0));

    // std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
    // std::cout << vel.x << " " << vel.y << " " << vel.z << std::endl;
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
        {
            a->setColliding(true);
            b->setColliding(true);
        }
        else
        {
            a->setColliding(false);
            b->setColliding(false); 
        }
        // collisions.emplace_back(a, b);
    }
}