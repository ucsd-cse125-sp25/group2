#include "camera.hpp"

Camera::Camera()
    : cameraPos(glm::vec3(0.0f, 2.0f, 5.0f)),
      cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)),
      cameraUp(glm::vec3(0.0f, 1.0f, 0.0f)), viewProjMat(glm::mat4(1.0f)) {
  fov = 60.0f;
  aspect = 1.33f;
  nearClip = 0.1f;
  farClip = 100.0f;

  yaw = -90.0f;
  pitch = 0.0f;
  lastX = 1920.0f / 2.0f;
  lastY = 1080.0f / 2.0f;

  firstMouse = true;
  sensitivity = 0.18f;
  radius = 13.0f;

  worldUp = cameraUp;
}

void Camera::update(float xpos, float ypos, glm::vec3 target) {
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  // restrict pitch (vertical) and allow 360 degrees for yaw (horizontal)
  pitch = glm::clamp(pitch, 0.0f, 30.0f);
  if (yaw > 360.0f || yaw < -360.0f)
    yaw = fmod(yaw, 360.0f);

  // Updating view projection matrix
  float camX = radius * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
  float camY = radius * sin(glm::radians(pitch));
  float camZ = radius * cos(glm::radians(pitch)) * sin(glm::radians(yaw));

  cameraPos = target + glm::vec3(camX, camY, camZ);

  glm::vec3 lookOffset(0.0f, 3.0f, 0.0f); // look a bit higher than the target
  glm::vec3 lookAtPoint = target + lookOffset;

  cameraFront = glm::normalize(lookAtPoint - cameraPos);

  cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
  cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));

  projection = glm::perspective(glm::radians(fov), aspect, nearClip, farClip);
  view = glm::lookAt(cameraPos, lookAtPoint, cameraUp);
  view = glm::lookAt(cameraPos, lookAtPoint, cameraUp);

  viewProjMat = projection * view;
}

void Camera::updateAspect(float width, float height) {
  aspect = width / height;
}