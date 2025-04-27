#pragma once

#include "client/core.hpp"
#include "glm/gtx/euler_angles.hpp"
class Camera
{
public:
  Camera();
  ~Camera();

  void update(float xpos, float ypos);

  // Getters
  glm::mat4 getViewProj() { return viewProjMat; }
  glm::vec3 getFacing() { return cameraFront; }
  glm::vec3 getPos() { return cameraPos; }
  glm::mat4 getProjection() { return projection; }
  glm::mat4 getView() { return view; }

private:
  // Perspective controls
  float FOV;      // Field of View Angle (degrees)
  float aspect;   // Aspect Ratio
  float nearClip; // Near clipping plane distance
  float farClip;  // Far clipping plane distance

  float yaw;
  float pitch;
  float lastX;
  float lastY;
  float fov;

  bool firstMouse;
  float sensitivity;

  glm::vec3 cameraPos;
  glm::vec3 cameraFront;
  glm::vec3 cameraUp;
  glm::vec3 cameraRight;
  glm::vec3 worldUp;

  glm::mat4 viewProjMat;
  glm::mat4 projection;
  glm::mat4 view;
};
