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

enum class InteractionType : uint8_t {
  NONE,
  PICKUP,
  PRESS,
  OPEN_CLOSE,
};

class GameObject : public BaseGameObject {
protected:
  // Server gameobject properties
  InteractionType interactable;

  // Physics properties
  bool grounded;
  unique_ptr<RigidBody> rigidbody;
  unique_ptr<Collider> collider;

public:
  GameObject(const int objectId, const bool isActive, unique_ptr<Transform> &tf,
             unique_ptr<RigidBody> &rb, unique_ptr<Collider> &cl)
      : BaseGameObject(objectId, isActive, tf), rigidbody(move(rb)),
        collider(move(cl)) {
    interactable = InteractionType::NONE;
  };

  // server
  void activate() { active = true; };
  void deactivate() { active = false; };
  void setInteractability(InteractionType interact) {
    interactable = interact;
  };
  InteractionType isInteractable() const { return interactable; };

  // physics
  void setGrounded(bool isGrounded) { grounded = isGrounded; };
  void setArea(float a) { getRigidBody()->setArea(a); };
  bool isGrounded() const { return grounded; };
  const float getArea() const { return getRigidBody()->getArea(); };
  RigidBody *getRigidBody() const { return rigidbody.get(); }
  Collider *getCollider() const { return collider.get(); }
};