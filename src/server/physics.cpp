#include "physics.hpp"

void Physics::add(GameObject *obj) { objects.push_back(obj); }

void Physics::remove(GameObject *obj) {
  objects.erase(find(objects.begin(), objects.end(), obj));
}

void Physics::calculateForces() {
  for (GameObject *obj : objects) {
    glm::vec3 vel = obj->getRigidBody()->getVelocity();
    glm::vec3 force = glm::vec3(0);
    if (!obj->isGrounded()) {
      force = obj->getRigidBody()->getMass() * gravity;
      if (glm::dot(vel, vel) > 0.001f)
        force += 0.5f * density * glm::dot(vel, vel) * drag *
                 obj->getRigidBody()->getArea() * -1.0f * glm::normalize(vel);
    }
    obj->getRigidBody()->applyForce(force);
    obj->setGrounded(false);
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
  const int solverIterations = 3;
  for (int i = 0; i < solverIterations; ++i) {
    for (GameObject *a : objects) {
      for (GameObject *b : objects) {
        if (a == b)
          continue;

        Collider *aCol = a->getCollider();
        Collider *bCol = b->getCollider();
        if (!aCol || !bCol)
          continue;

        glm::vec3 normal;
        float penetration;
        if (aCol->intersects(*bCol, normal, penetration)) {
          // if intersects, add both objects to the list of updated objects
          updatedObjects.insert(a->getId());
          updatedObjects.insert(b->getId());

          // Check if the object is at rest (grounded)
          bool aOnTop = glm::dot(normal, glm::vec3(0, 1, 0)) < 0.1f;
          bool bOnTop = glm::dot(normal, glm::vec3(0, -1, 0)) < 0.1f;
          if (aOnTop || bOnTop) {
            if (aOnTop)
              a->setGrounded(true);
            if (bOnTop)
              b->setGrounded(true);
          }
          // Get physics properties/variables
          RigidBody *a_rb = a->getRigidBody();
          RigidBody *b_rb = b->getRigidBody();
          clampVelocities(a_rb);
          clampVelocities(b_rb);
          glm::vec3 a_vel = a_rb->getVelocity();
          glm::vec3 b_vel = b_rb->getVelocity();
          float restitution =
              min(a_rb->getRestitution(), b_rb->getRestitution());
          float invMassA = a_rb->isStatic() ? 0.0f : 1.0f / a_rb->getMass();
          float invMassB = b_rb->isStatic() ? 0.0f : 1.0f / b_rb->getMass();
          float massSum = invMassA + invMassB;

          // Apply impulse based on mass and velocity of object's colliding.
          float v_close = glm::dot(a_vel - b_vel, normal);
          if (v_close < 0 && massSum > 0) {
            glm::vec3 impulse = -(1 + restitution) * v_close / massSum * normal;
            if (!a_rb->isStatic())
              a_rb->applyImpulse(-impulse * invMassA);
            if (!b_rb->isStatic())
              b_rb->applyImpulse(impulse * invMassB);
          }

          // Push objects out of each other
          const float percent = 0.1f;
          const float slop = 0.01f;
          if (massSum > 0) {
            glm::vec3 correction =
                max(penetration - slop, 0.0f) / massSum * percent * normal;
            if (!a_rb->isStatic())
              a->getTransform()->updatePosition(-correction * invMassA);
            if (!b_rb->isStatic())
              b->getTransform()->updatePosition(correction * invMassB);
          }
        }
      }
    }
  }
}

void Physics::moveObjects(float deltaTime) {
  float moveSpeed = 10.0f;

  for (GameObject *obj : objects) {
    if (obj->getRigidBody()->isStatic())
      continue;
    Transform *tf = obj->getTransform();
    RigidBody *rb = obj->getRigidBody();
    Collider *cl = obj->getCollider();
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
    cl->update(tf);
    rb->setForce(glm::vec3(0));

    // if object has moved, add it to the updated objects list
    if (glm::length(pos - lastPos) > 0.0001f)
      updatedObjects.insert(obj->getId());
  }
}