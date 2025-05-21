#pragma once

#include "core.hpp"
#include "globals.hpp"
#include "transform.hpp"

#include <memory>
#include <string>

using namespace std;

class BaseGameObject {
protected:
  OBJECT_ID id;
  bool active;
  unique_ptr<Transform> transform;

public:
  BaseGameObject(const OBJECT_ID id, const bool isActive,
                 unique_ptr<Transform> &tf)
      : id(id), active(isActive), transform(move(tf)){};

  virtual ~BaseGameObject(){};

  const OBJECT_ID getId() const { return id; };
  bool isActive() const { return active; };
  Transform *getTransform() { return transform.get(); };
  glm::vec3 getPosition() { return transform->getPosition(); };
  glm::vec3 getRotation() { return transform->getRotation(); };
  glm::vec3 getScale() { return transform->getScale(); };
};