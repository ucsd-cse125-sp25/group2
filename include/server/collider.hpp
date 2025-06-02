#pragma once

#include "core.hpp"
#include "transform.hpp"

#include <iostream>
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

  OBJECT_ID triggerObject = -1;

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
    glm::mat3 orient;
    orient[0] = tf->getRight();
    orient[1] = tf->getUp();
    orient[2] = -tf->getForward();
    center = orient * originalCenter + tf->getPosition();
    orientation = orient * originalOrientation;
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

  OBJECT_ID getTriggerObject() { return triggerObject; }
  void setTriggerObject(OBJECT_ID objID) { triggerObject = objID; }

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
  void drawWireframe() const {
    vector<glm::vec3> corners = getCorners();

    // Define the 12 edges of the box using indices into the corners vector
    static const int edges[12][2] = {
        {0, 1}, {1, 3}, {3, 2}, {2, 0}, // bottom face
        {4, 5}, {5, 7}, {7, 6}, {6, 4}, // top face
        {0, 4}, {1, 5}, {2, 6}, {3, 7}  // vertical edges
    };

    glDisable(GL_LIGHTING); // Disable lighting for clean line rendering
    glColor3f(1.0f, 0.0f,
              0.0f);   // Red color for the wireframe (you can customize)
    glLineWidth(1.5f); // Optional: set line thickness

    glBegin(GL_LINES);
    for (int i = 0; i < 12; ++i) {
      const glm::vec3 &a = corners[edges[i][0]];
      const glm::vec3 &b = corners[edges[i][1]];
      glVertex3f(a.x, a.y, a.z);
      glVertex3f(b.x, b.y, b.z);
    }
    glEnd();

    glEnable(GL_LIGHTING); // Re-enable lighting if you disabled it
  }
};