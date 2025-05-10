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
  sensitivity = 0.1f;
  speed = 2.5f;

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

  // restrict yaw (x-dir) and pitch (y-dir)
  yaw = glm::clamp(yaw, -110.0f, -70.0f);
  pitch = glm::clamp(pitch, -45.0f, 10.0f);

  // Updating view projection matrix
  // float radius = 5.0f;
  // float camX = radius * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
  // float camY = radius * sin(glm::radians(pitch));
  // float camZ = radius * cos(glm::radians(pitch)) * sin(glm::radians(yaw));

  // cameraPos = target + glm::vec3(camX, camY, camZ);
  // cameraFront = glm::normalize(target - cameraPos);

  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  cameraFront = glm::normalize(front);

  cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
  cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));

  projection = glm::perspective(glm::radians(fov), aspect, nearClip, farClip);
  view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

  viewProjMat = projection * view;
}

void Camera::updateAspect(float width, float height) {
  aspect = width / height;
}

void Camera::moveForward(float deltaTime) {
  glm::vec3 forwardDir = glm::normalize(
      glm::vec3(cameraFront.x, 0.0f, cameraFront.z)); // Flatten Y
  cameraPos += forwardDir * speed * deltaTime;
}

void Camera::moveBackward(float deltaTime) {
  glm::vec3 forwardDir =
      glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
  cameraPos -= forwardDir * speed * deltaTime;
}

void Camera::moveRight(float deltaTime) {
  glm::vec3 forwardDir =
      glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
  glm::vec3 rightDir =
      glm::normalize(glm::cross(forwardDir, glm::vec3(0.0f, 1.0f, 0.0f)));
  cameraPos += rightDir * speed * deltaTime;
}

void Camera::moveLeft(float deltaTime) {
  glm::vec3 forwardDir =
      glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
  glm::vec3 rightDir =
      glm::normalize(glm::cross(forwardDir, glm::vec3(0.0f, 1.0f, 0.0f)));
  cameraPos -= rightDir * speed * deltaTime;
}

glm::vec3 Camera::calculateRayFromCamera(float mouseX, float mouseY,
                                         float screenWidth,
                                         float screenHeight) {
  // Convert screen coordinates to normalized device coordinates (NDC)
  // NDC range: x,y from -1 to 1
  float ndcX = (2.0f * mouseX) / screenWidth - 1.0f;
  float ndcY = 1.0f - (2.0f * mouseY) / screenHeight; // Flip Y coordinate

  // Create a ray in clip space
  glm::vec4 rayClip = glm::vec4(ndcX, ndcY, -1.0f, 1.0f);

  // Convert to eye space using the inverse projection matrix
  glm::mat4 invProj = glm::inverse(getProjection());
  glm::vec4 rayEye = invProj * rayClip;
  rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f); // Reset Z and W

  // Convert to world space using the inverse view matrix
  glm::mat4 invView = glm::inverse(getView());
  glm::vec4 rayWorld = invView * rayEye;

  // Normalize the direction
  glm::vec3 rayDirection = glm::normalize(glm::vec3(rayWorld));

  return rayDirection;
}