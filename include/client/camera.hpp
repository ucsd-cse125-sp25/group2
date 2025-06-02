#pragma once

#include "core.hpp"
#include "globals.hpp"

#include <unordered_map>

using namespace std;

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

  unordered_map<PLAYER_ID, float>
      characterCamRadius; // Radius of each character for camera distance
  unordered_map<PLAYER_ID, glm::vec3>
      characterCamOffset; // Offset for each character camera position

public:
  Camera();

  void update(float xOffset, float yOffset, glm::vec3 target);
  void updateAspect(int width, int height);

  void setRadius(float newRadius) { radius = newRadius; }

  // Getters
  glm::vec3 getFacing() { return cameraFront; }
  glm::vec3 getPos() { return cameraPos; }
  glm::mat4 getProjection() { return projection; }
  glm::mat4 getView() { return view; }
  glm::mat4 getViewProj() { return viewProjMat; }
  glm::vec3 getRotation() { return glm::vec3(pitch, yaw, 0.0f); }
  float getCameraRadius(PLAYER_ID id) { return characterCamRadius[id]; }
  glm::vec3 getCameraOffset(PLAYER_ID id) {
    return characterCamOffset[id];
  }
};
