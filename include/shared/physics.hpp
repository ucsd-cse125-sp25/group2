#pragma once

#include <vector>
#include <algorithm>
#include "client/core.hpp"
#include "client/gameobject.hpp"

class Physics
{
    private:
        std::vector<GameObject*> objects;
        glm::vec3 gravity = glm::vec3(0, -9.81, 0);

    public:
        void Add(GameObject* obj);
        void Remove(GameObject* obj);
        void Update(float deltaTime);
};