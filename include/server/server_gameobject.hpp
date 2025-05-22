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
  bool usesGravity;
  bool grounded;
  unique_ptr<RigidBody> rigidbody;
  unique_ptr<Collider> collider;

public:
  GameObject(const OBJECT_ID id, const bool isActive, unique_ptr<Transform> &tf,
             unique_ptr<RigidBody> &rb, unique_ptr<Collider> &cl)
      : BaseGameObject(id, isActive, tf), rigidbody(move(rb)),
        collider(move(cl)) {
    interactionType = InteractionType::NONE;
    usesGravity = true;
    grounded = true;
  };

  void setInteractability(InteractionType interact) {
    interactionType = interact;
  };
  InteractionType getInteractionType() const { return interactionType; };

  // physics
  void setGrounded(bool isGrounded) { grounded = isGrounded; };
  void setUsesGravity(bool isAffected) { usesGravity = isAffected; };
  void setArea(float a) { getRigidBody()->setArea(a); };
  bool isGrounded() const { return grounded; };
  bool hasGravity() const { return usesGravity; };
  const float getArea() const { return getRigidBody()->getArea(); };
  RigidBody *getRigidBody() const { return rigidbody.get(); }
  Collider *getCollider() const { return collider.get(); }
};