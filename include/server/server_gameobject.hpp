#pragma once

#include "base_gameobject.hpp"
#include "collider.hpp"
#include "meshcollider.hpp"
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
  bool grounded;
  unique_ptr<RigidBody> rigidbody;
  std::vector<Collider*> colliders;
  unique_ptr<MeshCollider> meshcollider;

public:
  GameObject(const int objectId, const bool isActive, unique_ptr<Transform> &tf,
             unique_ptr<RigidBody> &rb, std::vector<Collider*>& cl, unique_ptr<MeshCollider> &mcl)
      : BaseGameObject(objectId, isActive, tf), rigidbody(move(rb)),
        colliders(move(cl)), meshcollider(move(mcl)) {
    interactionType = InteractionType::NONE;
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
  void setGrounded(bool isGrounded) { grounded = isGrounded; };
  void setArea(float a) { getRigidBody()->setArea(a); };
  bool isGrounded() const { return grounded; };
  const float getArea() const { return getRigidBody()->getArea(); };
  RigidBody *getRigidBody() const { return rigidbody.get(); }
  std::vector<Collider*> getCollider() const { return colliders; }
  MeshCollider *getMeshCollider() const { return meshcollider.get(); }

};