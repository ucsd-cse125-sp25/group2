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
    ResolveCollisions(deltaTime);
    for (GameObject* obj : this->objects)
    {
        Transform *tf = obj->getTransform();
        RigidBody *rb = obj->getRigidBody();
        Collider *cl = obj->getCollider();

        float halfHeight = 1;
        if (tf->getScale().x == 1) halfHeight = 1.75;
        else halfHeight = 1.025f;
        
        rb->applyForce(rb->getMass() * this->gravity);

        glm::vec3 vel = rb->getVelocity() + rb->getForce() / rb->getMass() * deltaTime;
        if (tf->getPosition().y <= halfHeight) vel = glm::vec3(0);
        rb->setVelocity(vel);

        glm::vec3 pos = tf->getPosition() + rb->getVelocity() * deltaTime;
        if (pos.y < halfHeight) pos.y = halfHeight;
        tf->setPosition(pos);
        cl->updatePosition(pos);
        rb->setForce(glm::vec3(0));

        // std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
        // std::cout << vel.x << " " << vel.y << " " << vel.z << std::endl;
    }
}

void Physics::ResolveCollisions(float deltaTime)
{
    std::vector<Collision> collisions;
    for (GameObject* a : objects)
    for (GameObject* b : objects) 
    {
        if (a == b) break;
        Collider* aBox = a->getCollider();
        Collider* bBox = b->getCollider();
        if (!aBox || !bBox) continue;

        // if (aBox->intersect(bBox)) 
        // {
        //     a->setColliding(true);
        //     b->setColliding(true);
        // }
        // else
        // {
        //     a->setColliding(false);
        //     b->setColliding(false); 
        // }
        // collisions.emplace_back(a, b);
    }
}