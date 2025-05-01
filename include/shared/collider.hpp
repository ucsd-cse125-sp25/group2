#pragma once

#include "shared/core.hpp"
#include "shared/objects/cube.hpp"

struct Plane {
  glm::vec3 A;
  glm::vec3 B;
  glm::vec3 C;
  glm::vec3 D;

  Plane(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d) {
    this->A = a;
    this->B = b;
    this->C = c;
    this->D = d;
  }
};

class Collider {
private:
  float xLength;
  float yLength;
  float zLength;
  glm::vec3 center;
  glm::vec3 boxMin;
  glm::vec3 boxMax;

public:
  // Cube* box;
  Collider(glm::vec3 ctr = glm::vec3(0)) { setCollider(ctr, 1, 1, 1); }

  Collider(glm::vec3 ctr, float x, float y, float z) {
    setCollider(ctr, x, y, z);
  }

  // void Update(glm::mat4 matrix)
  // {
  //     this->box->update(matrix);
  // }

  // void Draw(const glm::mat4& viewProjMtx, Shader& shader)
  // {
  //     this->box->draw(viewProjMtx, shader);
  // }

  bool intersect(Collider *other) {
    return (other->boxMin.x <= boxMax.x && other->boxMax.x >= boxMin.x &&
            other->boxMin.y <= boxMax.y && other->boxMax.y >= boxMin.y &&
            other->boxMin.z <= boxMax.z && other->boxMax.z >= boxMin.z);
  }

  void updatePosition(glm::vec3 pos) {
    setCollider(pos, this->xLength, this->yLength, this->zLength);
    // std::cout << this->center.x << " " << this->center.y << " " <<
    // this->center.z << std::endl;
  }

  void setCollider(glm::vec3 ctr, float x, float y, float z) {
    this->xLength = x;
    this->yLength = y;
    this->zLength = z;
    this->center = ctr;
    this->boxMin = ctr - glm::vec3(this->xLength, this->yLength, this->zLength);
    this->boxMax = ctr + glm::vec3(this->xLength, this->yLength, this->zLength);
    // if (this->box) delete this->box;
    // this->box = new Cube(-glm::vec3(this->xLength, this->yLength,
    // this->zLength), glm::vec3(this->xLength, this->yLength, this->zLength),
    // glm::vec3(0, 0.7, 0.7));
  }
};