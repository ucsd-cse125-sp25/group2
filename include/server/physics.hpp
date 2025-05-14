#pragma once

#include "core.hpp"
#include "server_gameobject.hpp"

#include <algorithm>
#include <unordered_map>
#include <vector>

using namespace std;

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
  vector<GameObject *> objects;
  glm::vec3 gravity = glm::vec3(0, -9.81, 0);
  float density = 1.225f;
  float drag = 1.05f;

public:
  void add(GameObject *obj);
  void remove(GameObject *obj);
  void update(float deltaTime);

  void calculateForces();
  void resolveCollisions();
  void moveObjects(float deltaTime);
};