#pragma once

#include "base_gameobject.hpp"
#include "collider.hpp"
#include "core.hpp"
#include "globals.hpp"
#include "press_functions.hpp"
#include "rigidbody.hpp"
#include "transform.hpp"

#include <iostream>
#include <memory>
#include <string>

using namespace std;

class GameObject : public BaseGameObject {
protected:
  InteractionType interactionType;
  function<void()> pressFunc;

  // Physics properties
  bool usesGravity;
  bool grounded;
  unique_ptr<RigidBody> rigidbody;
  vector<Collider *> colliders;

public:
  GameObject(const OBJECT_ID id, const LEVEL_ID levelNum, const bool isActive, unique_ptr<Transform> &tf,
             unique_ptr<RigidBody> &rb, vector<Collider *> &cl)
      : BaseGameObject(id, levelNum, isActive, tf), rigidbody(move(rb)), colliders(cl) {
    interactionType = InteractionType::NONE;
    usesGravity = true;
    grounded = true;
  };

  void setInteractability(InteractionType interact) {
    interactionType = interact;
  };
  InteractionType getInteractionType() const { return interactionType; };

  // if object is pressable
  void setPressFunction(function<void()> func) { pressFunc = func; };
  void press() {
    if (pressFunc) {
      pressFunc();
    } else {
      cerr << "No press function set for object ID: " << id << endl;
    }
  };

  // physics
  void setGrounded(bool isGrounded) { grounded = isGrounded; };
  void setUsesGravity(bool isAffected) { usesGravity = isAffected; };
  void setArea(float a) { getRigidBody()->setArea(a); };
  bool isGrounded() const { return grounded; };
  bool hasGravity() const { return usesGravity; };
  const float getArea() const { return getRigidBody()->getArea(); };
  RigidBody *getRigidBody() const { return rigidbody.get(); }
  vector<Collider *> getCollider() const { return colliders; }
};