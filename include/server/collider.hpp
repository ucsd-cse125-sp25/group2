#pragma once

#include "core.hpp"
#include "transform.hpp"
#include <vector>

using namespace std;

class Collider {
private:
  glm::vec3 originalCenter;
  glm::vec3 originalHalfExtents;
  glm::mat3 originalOrientation;

  glm::vec3 center;
  glm::vec3 halfExtents;
  glm::mat3 orientation;

  bool trigger = false;
  bool withinTrigger = false;
  bool activateTrigger = true;

  void projectOntoAxis(const glm::vec3 &axis, float &outMin,
                       float &outMax) const {
    vector<glm::vec3> corners = getCorners();
    outMin = outMax = glm::dot(axis, corners[0]);
    for (int i = 1; i < corners.size(); ++i) {
      float proj = glm::dot(axis, corners[i]);
      outMin = min(outMin, proj);
      outMax = max(outMax, proj);
    }
  }

public:
  Collider(glm::vec3 ctr = glm::vec3(0), glm::vec3 ext = glm::vec3(1.0f),
           glm::mat3 ort = glm::mat3(1))
      : originalCenter(ctr), center(ctr), originalHalfExtents(ext),
        halfExtents(ext), originalOrientation(ort), orientation(ort) {}

  void update(Transform *tf) {
    center = tf->getScale() * originalCenter + tf->getPosition();
    glm::mat3 orient;
    orient[0] = tf->getRight();
    orient[1] = tf->getUp();
    orient[2] = tf->getForward();
    orientation = orient * originalOrientation;
    halfExtents = tf->getScale() * originalHalfExtents;
  }

  glm::vec3 getCenter() const { return center; }
  glm::mat3 getOrientation() const { return orientation; }
  glm::vec3 getHalfExtents() const { return halfExtents; }
  vector<glm::vec3> getCorners() const {
    vector<glm::vec3> corners;
    for (int x = -1; x <= 1; x += 2)
      for (int y = -1; y <= 1; y += 2)
        for (int z = -1; z <= 1; z += 2)
          corners.push_back(center +
                            orientation * (halfExtents * glm::vec3(x, y, z)));
    return corners;
  }
  bool isTrigger() { return trigger; }
  void setTrigger(bool isTrigger) { trigger = isTrigger; }

  bool isWithinTrigger() { return withinTrigger; }
  void setWithinTrigger(bool insideTrigger) { withinTrigger = insideTrigger; }

  bool canActivateTrigger() { return activateTrigger; }
  void setCanActivate(bool activate) { activateTrigger = activate; }

  bool intersects(const Collider &other, glm::vec3 &outNormal,
                  float &outPenetration) const {
    const float epsilon = 1e-5f;
    float minPenetration = numeric_limits<float>::infinity();
    glm::vec3 bestAxis = glm::vec3(0.0f);

    vector<glm::vec3> axes;
    for (int i = 0; i < 3; ++i)
      axes.push_back(glm::normalize(orientation[i]));
    for (int i = 0; i < 3; ++i)
      axes.push_back(glm::normalize(other.orientation[i]));
    for (int i = 0; i < 3; ++i) {
      glm::vec3 axisA = orientation[i];
      for (int j = 0; j < 3; ++j) {
        glm::vec3 axisB = other.orientation[j];
        glm::vec3 cross = glm::cross(axisA, axisB);
        if (glm::length(cross) > epsilon)
          axes.push_back(glm::normalize(cross));
      }
    }

    for (const glm::vec3 &axis : axes) {
      float aMin, aMax, bMin, bMax;
      projectOntoAxis(axis, aMin, aMax);
      other.projectOntoAxis(axis, bMin, bMax);

      float overlap = min(aMax, bMax) - max(aMin, bMin);
      if (overlap <= 0) {
        return false;
      }

      if (overlap < minPenetration) {
        minPenetration = overlap;
        bestAxis = axis;
        if (glm::dot(axis, other.center - center) < 0)
          bestAxis = -axis;
      }
    }

    outNormal = bestAxis;
    outPenetration = minPenetration;
    return true;
  }
};