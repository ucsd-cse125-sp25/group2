#pragma once

#include "base_gameobject.hpp"
#include "core.hpp"
#include "globals.hpp"
#include "transform.hpp"

#include <memory>
#include <string>

using namespace std;

enum class InteractionType : uint8_t {
  NONE,
  PICKUP,
  PRESS,
  OPEN_CLOSE,
};

class GameObject : public BaseGameObject {
protected:
  InteractionType interactable;

  // physics properties
  float velocity;

public:
  GameObject(const int objectId, const bool isActive, unique_ptr<Transform> &tf)
      : BaseGameObject(objectId, isActive, tf) {
    interactable = InteractionType::NONE;
    velocity = 0.1f;
  };

  virtual ~GameObject(){};

  void activate() { active = true; };
  void deactivate() { active = false; };
  void setInteractability(InteractionType interact) {
    interactable = interact;
  };
  InteractionType isInteractable() const { return interactable; };

  // physics methods
  void applyMovement(const glm::vec3 &direction);
};