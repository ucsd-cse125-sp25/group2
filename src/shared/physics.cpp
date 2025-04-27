#include "shared/physics.hpp"
#include <iostream>

void Physics::Add(GameObject* obj)
{
    this->objects.push_back(obj);
}

void Physics::Remove(GameObject* obj)
{
    this->objects.erase(std::remove(this->objects.begin(), this->objects.end(), obj));
}

void Physics::Update(float deltaTime)
{
    for (GameObject* obj : this->objects)
    {
        RigidBody* rb = obj->getRigidBody();
        Transform* tf = obj->getTransform();
        float halfHeight = 2.05f;

        rb->applyForce(rb->getMass() * this->gravity);

        glm::vec3 vel = rb->getVelocity() + rb->getForce() / rb->getMass() * deltaTime;
        if (tf->getPosition().y <= halfHeight) vel = glm::vec3(0);
        rb->setVelocity(vel);

        glm::vec3 pos = tf->getPosition() + rb->getVelocity() * deltaTime;
        if (pos.y < halfHeight) pos.y = halfHeight;
        tf->setPosition(pos);
        
        rb->setForce(glm::vec3(0));

        // std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
        // std::cout << vel.x << " " << vel.y << " " << vel.z << std::endl;
    }
}