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

  vec3 cameraPos;
  vec3 cameraFront;
  vec3 cameraUp;
  vec3 cameraRight;
  vec3 worldUp;

  mat4 projection;
  mat4 view;
  mat4 viewProjMat;

public:
  Camera();

  void update(float xOffset, float yOffset, vec3 target);
  void updateAspect(int width, int height);

  // Getters
  vec3 getFacing() { return cameraFront; }
  vec3 getPos() { return cameraPos; }
  mat4 getProjection() { return projection; }
  mat4 getView() { return view; }
  mat4 getViewProj() { return viewProjMat; }
  vec3 getRotation() { return vec3(pitch, yaw, 0.0f); }
};
