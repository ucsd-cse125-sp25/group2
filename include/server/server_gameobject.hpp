#pragma once

#include "base_gameobject.hpp"
#include "core.hpp"
#include "globals.hpp"
#include "transform.hpp"

#include <memory>
#include <string>

using namespace std;

class GameObject : public BaseGameObject {
protected:
  InteractionType interactionType;

  // physics properties
  float velocity;

public:
  GameObject(const int objectId, const bool isActive, unique_ptr<Transform> &tf)
      : BaseGameObject(objectId, isActive, tf) {
    interactionType = InteractionType::NONE;
    velocity = 0.1f;
  };

  virtual ~GameObject(){};

  void activate() { active = true; };
  void deactivate() { active = false; };
  void setInteractability(InteractionType interactionType) {
    interactionType = interactionType;
  };
  InteractionType getInteractionType() const { return interactionType; };

  // physics methods
  void applyMovement(const glm::vec3 &direction);
};