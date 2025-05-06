#pragma once

#include "client/shader.hpp"
#include "shared/core.hpp"
#include "shared/objects/cube.hpp"
#include "shared/transform.hpp"
#include <memory>
#include <vector>

using namespace std;

class Cube {
private:
  GLuint VAO;
  GLuint VBO_positions, VBO_normals, EBO;

  glm::mat4 model;
  glm::vec3 color;

  // Cube Information
  vector<glm::vec3> positions;
  vector<glm::vec3> normals;
  vector<unsigned int> indices;

public:
  Cube(glm::vec3 cubeMin = glm::vec3(-1, -1, -1),
       glm::vec3 cubeMax = glm::vec3(1, 1, 1));
  ~Cube();

  void draw(const glm::mat4 &viewProjMtx, unique_ptr<Shader> &shader);
  // void update(Transform *transform);
  void update();

  void spin(float deg);
};