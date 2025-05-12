#pragma once

#include "shared/core.hpp"
#include "shared/gameobject.hpp"
#include <unordered_map>
#include <algorithm>
#include <vector>

struct Collision {
  GameObject *a;
  GameObject *b;

  Collision(GameObject *a, GameObject *b) {
    this->a = a;
    this->b = b;
  }
};

class Physics {
private:
  std::vector<GameObject *> objects;
  glm::vec3 gravity = glm::vec3(0, -9.81, 0);
  float density = 1.225f;
  float drag = 1.05f;

public:
  void Add(GameObject *obj);
  void Remove(GameObject *obj);
  void Update(float deltaTime);

  void CalculateForces();
  void ResolveCollisions();
  void MoveObjects(float deltaTime);
};