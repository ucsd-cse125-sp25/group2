#include "camera.hpp"

Camera::Camera()
    : cameraPos(glm::vec3(0.0f, 2.0f, 5.0f)),
      cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)),
      cameraUp(glm::vec3(0.0f, 1.0f, 0.0f)), viewProjMat(glm::mat4(1.0f)) {
  fov = 60.0f;
  aspect = 1.33f;
  nearClip = 0.1f;
  farClip = 100.0f;

  yaw = 90.0f; // place camera along +Z axis to see behind the target
  pitch = 0.0f;

  sensitivity = 0.1f;
  radius = 10.0f; // default distance from the target

  worldUp = cameraUp;

  characterCamRadius[CHICKEN] = 3.0f;
  characterCamRadius[PIG] = 5.5f;
  characterCamRadius[SHEEP] = 6.0f;
  characterCamRadius[COW] = 6.0f;

  characterCamOffset[CHICKEN] = glm::vec3(0.0f, 2.0f, 0.0f);
  characterCamOffset[PIG] = glm::vec3(0.0f, 3.0f, 0.0f);
  characterCamOffset[SHEEP] = glm::vec3(0.0f, 4.3f, 0.0f);
  characterCamOffset[COW] = glm::vec3(0.0f, 4.8f, 0.0f);
}

void Camera::update(float xOffset, float yOffset, glm::vec3 target) {
  xOffset *= sensitivity;
  yOffset *= sensitivity;

  yaw += xOffset;
  pitch -= yOffset;

  yaw = fmod(yaw, 360.0f);
  if (yaw < 0.0f)
    yaw += 360.0f;

  // restrict pitch (vertical)
  pitch = glm::clamp(pitch, -20.0f, 45.0f);

  // Updating view projection matrix
  float camX = radius * cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  float camY = radius * sin(glm::radians(pitch));
  float camZ = radius * sin(glm::radians(yaw)) * cos(glm::radians(pitch));

  cameraPos = target + glm::vec3(camX, camY, camZ);

  cameraFront = glm::normalize(target - cameraPos);

  cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
  cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));

  projection = glm::perspective(glm::radians(fov), aspect, nearClip, farClip);
  view = glm::lookAt(cameraPos, target, cameraUp);

  viewProjMat = projection * view;
}

void Camera::updateAspect(int width, int height) {
  aspect = static_cast<float>(width) / height;
}