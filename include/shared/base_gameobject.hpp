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
  LEVEL_ID level;
  bool active;
  glm::vec3 originalPosition;
  unique_ptr<Transform> transform;

public:
  BaseGameObject(const OBJECT_ID id, const int levelNum, const bool isActive,
                 glm::vec3 originalPos, unique_ptr<Transform> &tf)
      : id(id), level(levelNum), active(isActive),
        originalPosition(originalPos), transform(move(tf)){};

  virtual ~BaseGameObject(){};

  // setters
  void activate() { active = true; };
  void deactivate() { active = false; };

  // getters
  const OBJECT_ID getId() const { return id; };
  bool isActive() const { return active; };
  LEVEL_ID getLevelID() const { return level; };
  glm::vec3 getOriginalPosition() { return originalPosition; }
  Transform *getTransform() { return transform.get(); };
  glm::vec3 getPosition() { return transform->getPosition(); };
  glm::vec3 getRotation() { return transform->getRotation(); };
  glm::vec3 getScale() { return transform->getScale(); };
};