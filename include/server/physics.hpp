#pragma once

#include "core.hpp"
#include "server_gameobject.hpp"

#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

struct Collision {
  GameObject *a;
  GameObject *b;

  Collision(GameObject *objA, GameObject *objB) {
    a = objA;
    b = objB;
  }
};

class Physics {
private:
  vector<GameObject *> objects;
  const glm::vec3 gravity = glm::vec3(0, -9.8 * 2, 0);
  const float density = 1.225f;
  const float drag = 1.05f;
  const float damping = 0.98f;

  unordered_set<int> updatedObjects;

  void clampVelocities(RigidBody *rb);

public:
  void add(GameObject *obj);
  void remove(GameObject *obj);

  void calculateForces();
  void resolveCollisions();
  void solveCollision(GameObject *a, GameObject *b, int aIndex, int bIndex,
                      bool &aGrounded, bool &bGrounded);
  void moveObjects(float deltaTime);

  vector<int> getUpdatedObjects() {
    vector<int> list(updatedObjects.begin(), updatedObjects.end());
    updatedObjects.clear();
    return list;
  }
};