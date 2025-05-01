#pragma once

#include "shared/core.hpp"
#include "shared/gameobject.hpp"
#include <algorithm>
#include <vector>

struct Collision {
  GameObject *a;
  GameObject *b;

  Collision(GameObject *a, GameObject *b) { this->a = a; this->b = b; }
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
  void ResolveCollisions(float deltaTime);
  void SolveCollisions(std::vector<Collision> collisions, float deltaTime);
  void Solve(Collision collision, float deltaTime);
  void MoveObjects(float deltaTime);
};