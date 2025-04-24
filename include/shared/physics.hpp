#pragma once

#include <vector>
#include <algorithm>
#include "client/core.hpp"

struct Object
{
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 force;
    float mass;

    Object(glm::vec3 p = glm::vec3(0), glm::vec3 v = glm::vec3(0), glm::vec3 f = glm::vec3(0), float m = 1)
    {
        this->position = p;
        this->velocity = v;
        this->force = f;
        this->mass = m;
    }
};

class Physics
{
    private:
        std::vector<Object*> objects;
        glm::vec3 gravity = glm::vec3(0, -9.81, 0);

    public:
        void Add(Object* obj);
        void Remove(Object* obj);
        void Update(float deltaTime);
};