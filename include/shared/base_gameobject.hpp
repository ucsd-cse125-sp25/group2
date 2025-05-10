#pragma once

#include "core.hpp"
#include "transform.hpp"
#include "globals.hpp"

#include <memory>
#include <string>

using namespace std;

class BaseGameObject {
protected:
  int id;
  bool active;
  unique_ptr<Transform> transform;

public:
  BaseGameObject(const int objectId, const bool isActive,
                 unique_ptr<Transform> &tf)
      : id(objectId), active(isActive), transform(move(tf)){};

  virtual ~BaseGameObject(){};

  const int getId() const { return id; };
  bool isActive() const { return active; };
  Transform *getTransform() { return transform.get(); };
  glm::vec3 getPosition() { return transform->getPosition(); };
  glm::vec3 getRotation() { return transform->getRotation(); };
  glm::vec3 getScale() { return transform->getScale(); };
};