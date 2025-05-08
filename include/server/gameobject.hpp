#pragma once

#include "shared/core.hpp"
#include "shared/transform.hpp"
#include "shared/types.hpp"
#include "shared/base_gameobject.hpp"

#include <memory>
#include <string>

using namespace std;

class GameObject : public BaseGameObject {
protected:
  bool interactable;

  // physics properties
  float velocity;

public:
  GameObject(const int objectId, const bool isActive, unique_ptr<Transform> &tf)
      : BaseGameObject(objectId, isActive, tf){
    interactable = false;
    velocity = 0.1f;
  };

  virtual ~GameObject(){};

  void activate() { active = true; };
  void deactivate() { active = false; };
  void setInteractability(bool canInteract) { interactable = canInteract; };
  bool isInteractable() const { return interactable; };

  // physics methods
  void applyMovement(const glm::vec3 &direction);
};