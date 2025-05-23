#pragma once

#include "core.hpp"
#include "glm/gtx/euler_angles.hpp"
class Camera {
private:
  // Perspective controls
  float fov;      // Field of View Angle (degrees)
  float aspect;   // Aspect Ratio
  float nearClip; // Near clipping plane distance
  float farClip;  // Far clipping plane distance

  float yaw;
  float pitch;

  float sensitivity;
  float radius; // Distance from the camera to the target

  glm::vec3 cameraPos;
  glm::vec3 cameraFront;
  glm::vec3 cameraUp;
  glm::vec3 cameraRight;
  glm::vec3 worldUp;

  glm::mat4 projection;
  glm::mat4 view;
  glm::mat4 viewProjMat;

public:
  Camera();

  void update(float xOffset, float yOffset, glm::vec3 target);
  void updateAspect(int width, int height);

  // Getters
  glm::vec3 getFacing() { return cameraFront; }
  glm::vec3 getPos() { return cameraPos; }
  glm::mat4 getProjection() { return projection; }
  glm::mat4 getView() { return view; }
  glm::mat4 getViewProj() { return viewProjMat; }
  glm::vec3 getRotation() { return glm::vec3(pitch, yaw, 0.0f); }
};
