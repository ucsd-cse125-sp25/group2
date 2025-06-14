#include "physics.hpp"

void Physics::add(GameObject *obj) { objects.push_back(obj); }

void Physics::remove(GameObject *obj) {
  objects.erase(find(objects.begin(), objects.end(), obj));
}

void Physics::calculateForces() {
  for (GameObject *obj : objects) {
    if (obj->getRigidBody()->isStatic() || !obj->isActive())
      continue;

    glm::vec3 force = glm::vec3(0);

    if (!obj->isGrounded()) {
      if (obj->hasGravity())
        force += obj->getRigidBody()->getMass() * gravity;
    }
    obj->getRigidBody()->applyForce(force);
  }
}

void Physics::clampVelocities(RigidBody *rb) {
  glm::vec3 v = rb->getVelocity();
  if (glm::dot(v, v) < 0.001f) {
    rb->setVelocity(glm::vec3(0.0f));
    return;
  }
  rb->setVelocity(v * damping);
}

void Physics::resolveCollisions() {
  // Multiple iterations smooths out collision resolution fixes
  const int solverIterations = 9;
  unordered_map<GameObject *, bool> groundedStates;
  for (auto obj : objects) {
    groundedStates[obj] = false;
  }
  for (int s = 0; s < solverIterations; ++s) {
    for (int i = 0; i < objects.size(); ++i) {
      GameObject *a = objects[i];
      for (int j = i + 1; j < objects.size(); ++j) {
        GameObject *b = objects[j];
        if (!a->isActive() || !b->isActive())
          continue;
        for (int i = 0; i < a->getCollider().size(); i++) {
          for (int j = 0; j < b->getCollider().size(); j++) {
            solveCollision(a, b, i, j, groundedStates[a], groundedStates[b]);
          }
        }
      }
    }
  }
  for (auto &[obj, isGrounded] : groundedStates) {
    obj->setGrounded(isGrounded);
  }
}

void Physics::solveCollision(GameObject *a, GameObject *b, int aIndex,
                             int bIndex, bool &aGrounded, bool &bGrounded) {
  Collider *aCol = a->getCollider()[aIndex];
  Collider *bCol = b->getCollider()[bIndex];

  if (!aCol || !bCol || !a->isActive() || !b->isActive())
    return;

  glm::vec3 normal;
  float penetration;
  if (aCol->intersects(*bCol, normal, penetration)) {
    if (aCol->isTrigger() || bCol->isTrigger()) {
      if (aCol->isTrigger() && bCol->canActivateTrigger()) {
        aCol->setWithinTrigger(true);
        aCol->setTriggerObject(b->getID());
      }
      if (bCol->isTrigger() && aCol->canActivateTrigger()) {
        bCol->setWithinTrigger(true);
        bCol->setTriggerObject(a->getID());
      }
      return;
    }

    updatedObjects.insert(a->getID());
    updatedObjects.insert(b->getID());

    // Check if the object is at rest (grounded)
    float groundThreshold = 0.7f;
    if (normal.y > groundThreshold) {
      bGrounded = true;
    } else if (normal.y < -groundThreshold) {
      aGrounded = true;
    }

    // Get physics properties/variables
    RigidBody *a_rb = a->getRigidBody();
    RigidBody *b_rb = b->getRigidBody();
    glm::vec3 a_vel = a_rb->getVelocity();
    glm::vec3 b_vel = b_rb->getVelocity();
    float restitution = min(a_rb->getRestitution(), b_rb->getRestitution());

    bool aIsNotPlayer = a->getID() >= NUM_PLAYERS;
    bool bIsNotPlayer = b->getID() >= NUM_PLAYERS;

    // Objects cannot be moved by any player except for a cow
    bool aIsStatic = b->getID() < COW && aIsNotPlayer ? true : a_rb->isStatic();
    bool bIsStatic = a->getID() < COW && bIsNotPlayer ? true : b_rb->isStatic();

    // If this object is being held by this player, we don't want it to collide
    // with it
    aIsStatic = a->getID() == b_rb->playerHold() ? true : aIsStatic;
    bIsStatic = b->getID() == a_rb->playerHold() ? true : bIsStatic;

    // If both objects are static, no need to resolve collision
    if (aIsStatic && bIsStatic) {
      return;
    }

    float invMassA = aIsStatic ? 0.0f : 1.0f / a_rb->getMass();
    float invMassB = bIsStatic ? 0.0f : 1.0f / b_rb->getMass();
    float massSum = invMassA + invMassB;

    // Apply impulse based on mass and velocity of object's colliding.
    float v_close = glm::dot(b_vel - a_vel, normal);
    if (v_close < 0 && massSum > 0) {
      glm::vec3 impulse = -(1 + restitution) * v_close / massSum * normal;
      if (!aIsStatic)
        a_rb->applyImpulse(-impulse * invMassA);
      if (!bIsStatic)
        b_rb->applyImpulse(impulse * invMassB);
    }

    // Push objects out of each other
    const float percent = 0.1f;
    const float slop = 0.01f;
    if (massSum > 0) {
      glm::vec3 correction =
          max(penetration - slop, 0.0f) / massSum * percent * normal;
      float sheepBounce = 15.0f;
      if (!a_rb->isStatic()) {
        a->getTransform()->updatePosition(-correction * invMassA);
        if (b->getID() == SHEEP && normal.y < -0.7) {
          a_rb->applyImpulse(sheepBounce * glm::vec3(0, 1, 0) *
                             a_rb->getMass());
        }
        for (Collider *c : a->getCollider()) {
          c->update(a->getTransform());
        }
      }
      if (!bIsStatic) {
        b->getTransform()->updatePosition(correction * invMassB);
        if (a->getID() == SHEEP && normal.y > 0.7) {
          b_rb->applyImpulse(sheepBounce * glm::vec3(0, 1, 0) *
                             b_rb->getMass());
        }
        for (Collider *c : b->getCollider()) {
          c->update(b->getTransform());
        }
      }
    }
  }
}

void Physics::moveObjects(float deltaTime) {
  float moveSpeed = 50.0f;

  for (GameObject *obj : objects) {
    if (!obj->isActive())
      continue;
    RigidBody *rb = obj->getRigidBody();
    vector<Collider *> cl = obj->getCollider();
    if (cl[0]->isTrigger()) {
      cl[0]->setWithinTrigger(false);
    }
    if (rb->isStatic())
      continue;
    Transform *tf = obj->getTransform();
    glm::vec3 lastPos = tf->getPosition();

    glm::vec3 vel =
        rb->getVelocity() + rb->getForce() / rb->getMass() * deltaTime;
    // Limit velocity to maxSpeed (Otherwise you go super fast mid air)
    glm::vec3 flatVel = glm::vec3(vel.x, 0, vel.z);
    if (glm::length(flatVel) > moveSpeed) {
      glm::vec3 limitVel = moveSpeed * glm::normalize(flatVel);
      vel = glm::vec3(limitVel.x, vel.y, limitVel.z);
    }
    rb->setVelocity(vel);
    glm::vec3 pos = tf->getPosition() + rb->getVelocity() * deltaTime;
    tf->setPosition(pos);
    for (Collider *c : cl) {
      c->update(tf);
    }
    rb->setForce(glm::vec3(0.0f));
    rb->setVelocity(glm::vec3(0, vel.y, 0));

    // if object has moved, add it to the updated objects list
    if (glm::length(pos - lastPos) > 0.0001f)
      updatedObjects.insert(obj->getID());
  }
}