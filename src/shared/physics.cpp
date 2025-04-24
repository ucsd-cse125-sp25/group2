#include "shared/physics.hpp"
#include <iostream>

void Physics::Add(Object* obj)
{
    this->objects.push_back(obj);
}

void Physics::Remove(Object* obj)
{
    this->objects.erase(std::remove(this->objects.begin(), this->objects.end(), obj));
}

void Physics::Update(float deltaTime)
{
    for (Object* obj : this->objects)
    {
        obj->force += obj->mass * this->gravity;
        obj->velocity += obj->force / obj->mass * deltaTime;
        obj->position += obj->velocity * deltaTime;
        
        obj->force = glm::vec3(0);
    }
}