#pragma once

#include "base_gameobject.hpp"
#include "collider.hpp"
#include "core.hpp"
#include "globals.hpp"
#include "rigidbody.hpp"
#include "transform.hpp"

#include <memory>
#include <string>

using namespace std;

class GameObject : public BaseGameObject {
protected:
  InteractionType interactionType;

  // Physics properties
  bool disablePhysics;
  bool grounded;
  unique_ptr<RigidBody> rigidbody;
  unique_ptr<Collider> collider;

public:
  GameObject(const OBJECT_ID id, const bool isActive, unique_ptr<Transform> &tf,
             unique_ptr<RigidBody> &rb, unique_ptr<Collider> &cl)
      : BaseGameObject(id, isActive, tf), rigidbody(move(rb)),
        collider(move(cl)) {
    interactionType = InteractionType::NONE;
    disablePhysics = false;
    grounded = true;
  };

  // server
  void activate() { active = true; };
  void deactivate() { active = false; };
  void setInteractability(InteractionType interactionType) {
    interactionType = interactionType;
  };
  InteractionType getInteractionType() const { return interactionType; };

  // physics
  void setDisable(bool isDisabled) { disablePhysics = isDisabled; };
  void setGrounded(bool isGrounded) { grounded = isGrounded; };
  void setArea(float a) { getRigidBody()->setArea(a); };
  bool isGrounded() const { return grounded; };
  bool isDisabled() const { return disablePhysics; };
  const float getArea() const { return getRigidBody()->getArea(); };
  RigidBody *getRigidBody() const { return rigidbody.get(); }
  Collider *getCollider() const { return collider.get(); }
};