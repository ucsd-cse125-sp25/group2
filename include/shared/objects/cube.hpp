#pragma once

#include "gameobject.hpp"

class Cube : public GameObject {
public:
  Cube(const int objectId, const bool isActive, unique_ptr<Transform> &tf)
      : GameObject(objectId, isActive, tf) {
    type = ObjectType::CUBE;
  }
};