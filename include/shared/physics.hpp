#pragma once

#include "shared/core.hpp"
#include "shared/gameobject.hpp"
#include <algorithm>
#include <vector>

struct Collision {
  GameObject *a;
  GameObject *b;
};

class Physics {
private:
  std::vector<GameObject *> objects;
  glm::vec3 gravity = glm::vec3(0, -9.81, 0);

public:
  void Add(GameObject *obj);
  void Remove(GameObject *obj);
  void Update(float deltaTime);

  void ResolveCollisions(float deltaTime);
};