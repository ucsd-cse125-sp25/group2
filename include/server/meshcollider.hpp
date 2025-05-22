#pragma once

#include "core.hpp"
#include "transform.hpp"
#include <array>
#include <filesystem>
#include <iostream>
#include <tiny_obj_loader.h>
#include <vector>

struct Triangle {
  glm::vec3 v0, v1, v2;
};

struct Contact {
  glm::vec3 normal = glm::vec3(0.0f);
  float penetration = 0.0f;
  glm::vec3 contactPoint = glm::vec3(0.0f);
};

class MeshCollider {
private:
  std::vector<Triangle> triangles;
  glm::mat4 model;

  void LoadFromObj(const std::string &path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    std::filesystem::path filepath = path;
    std::string base_dir = filepath.parent_path().string();
    std::string file_name = filepath.filename().string();

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                                (base_dir + "/" + file_name).c_str(),
                                base_dir.c_str());

    if (!warn.empty())
      std::cerr << "WARN: " << warn << std::endl;
    if (!err.empty())
      std::cerr << "ERR: " << err << std::endl;
    if (!ret)
      throw std::runtime_error("Failed to load OBJ file");

    for (const auto &shape : shapes) {
      size_t index_offset = 0;
      for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
        int fv = shape.mesh.num_face_vertices[f];
        if (fv != 3)
          continue;

        glm::vec3 verts[3];
        for (size_t v = 0; v < 3; v++) {
          auto idx = shape.mesh.indices[index_offset + v];
          verts[v] = glm::vec3(attrib.vertices[3 * idx.vertex_index + 0],
                               attrib.vertices[3 * idx.vertex_index + 1],
                               attrib.vertices[3 * idx.vertex_index + 2]);
        }

        triangles.push_back({verts[0], verts[1], verts[2]});
        index_offset += fv;
      }
    }
  }

  Triangle TransformTriangle(const Triangle &tri, const glm::mat4 &mat) const {
    return {glm::vec3(mat * glm::vec4(tri.v0, 1.0f)),
            glm::vec3(mat * glm::vec4(tri.v1, 1.0f)),
            glm::vec3(mat * glm::vec4(tri.v2, 1.0f))};
  }

  bool CheckAABB(MeshCollider *other) {
    glm::vec3 thisMin, thisMax;
    CalculateAABB(thisMin, thisMax);

    glm::vec3 otherMin, otherMax;
    other->CalculateAABB(otherMin, otherMax);

    return (thisMin.x <= otherMax.x && thisMax.x >= otherMin.x) &&
           (thisMin.y <= otherMax.y && thisMax.y >= otherMin.y) &&
           (thisMin.z <= otherMax.z && thisMax.z >= otherMin.z);
  }

  void CalculateAABB(glm::vec3 &min, glm::vec3 &max) {
    if (triangles.empty()) {
      min = max = glm::vec3(0.0f);
      return;
    }

    Triangle firstTri = TransformTriangle(triangles[0], model);
    min = max = firstTri.v0;

    for (const auto &tri : triangles) {
      Triangle worldTri = TransformTriangle(tri, model);
      glm::vec3 vertices[3] = {worldTri.v0, worldTri.v1, worldTri.v2};
      for (int i = 0; i < 3; i++) {
        min.x = std::min(min.x, vertices[i].x);
        min.y = std::min(min.y, vertices[i].y);
        min.z = std::min(min.z, vertices[i].z);

        max.x = std::max(max.x, vertices[i].x);
        max.y = std::max(max.y, vertices[i].y);
        max.z = std::max(max.z, vertices[i].z);
      }
    }
  }

  bool TriangleTriangleIntersect(const Triangle &triA, const Triangle &triB,
                                 Contact &contact) {
    glm::vec3 edgesA[3] = {triA.v1 - triA.v0, triA.v2 - triA.v1,
                           triA.v0 - triA.v2};
    glm::vec3 edgesB[3] = {triB.v1 - triB.v0, triB.v2 - triB.v1,
                           triB.v0 - triB.v2};

    glm::vec3 normalA = glm::cross(edgesA[0], -edgesA[2]);
    glm::vec3 normalB = glm::cross(edgesB[0], -edgesB[2]);
    if (glm::length(normalA) < 1e-6f || glm::length(normalB) < 1e-6f) {
      return false;
    }
    normalA = glm::normalize(normalA);
    normalB = glm::normalize(normalB);

    std::vector<std::pair<glm::vec3, bool>> testAxes;
    testAxes.push_back({normalA, true});
    testAxes.push_back({normalB, true});
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        glm::vec3 axis = glm::cross(edgesA[i], edgesB[j]);
        float length = glm::length(axis);
        if (length > 1e-6f) {
          testAxes.push_back({axis / length, false});
        }
      }
    }

    float minOverlap = std::numeric_limits<float>::max();
    float minFaceOverlap = std::numeric_limits<float>::max();
    glm::vec3 bestAxis;
    glm::vec3 bestFaceAxis;
    bool foundFaceCollision = false;
    for (const auto &axisInfo : testAxes) {
      const glm::vec3 &axis = axisInfo.first;
      bool isFaceNormal = axisInfo.second;
      float minA, maxA, minB, maxB;
      ProjectTriangleOntoAxis(triA, axis, minA, maxA);
      ProjectTriangleOntoAxis(triB, axis, minB, maxB);

      const float tolerance = 1e-6f;
      if (maxA < minB - tolerance || maxB < minA - tolerance) {
        return false; // Separating axis found
      }

      float overlap = std::min(maxA - minB, maxB - minA);
      if (overlap > tolerance) {
        if (overlap < minOverlap) {
          minOverlap = overlap;
          bestAxis = axis;
        }
        if (isFaceNormal && overlap < minFaceOverlap) {
          minFaceOverlap = overlap;
          bestFaceAxis = axis;
          foundFaceCollision = true;
        }
      }
    }
    if (minOverlap >= std::numeric_limits<float>::max() ||
        minOverlap <= 1e-6f) {
      return false;
    }

    glm::vec3 finalNormal;
    float finalPenetration;
    if (foundFaceCollision && (minFaceOverlap <= minOverlap * 1.1f)) {
      finalNormal = bestFaceAxis;
      finalPenetration = minFaceOverlap;
    } else {
      return false;
    }

    glm::vec3 centroidA = (triA.v0 + triA.v1 + triA.v2) / 3.0f;
    glm::vec3 centroidB = (triB.v0 + triB.v1 + triB.v2) / 3.0f;
    glm::vec3 direction = centroidB - centroidA;
    if (glm::dot(finalNormal, direction) < 0) {
      finalNormal = -finalNormal;
    }

    contact.normal = finalNormal;
    contact.penetration = finalPenetration;
    contact.contactPoint = CalculateContactPoint(triA, triB);
    std::cout << contact.normal.x << " " << contact.normal.y << " "
              << contact.normal.z << std::endl;
    return true;
  }

  void ProjectTriangleOntoAxis(const Triangle &tri, const glm::vec3 &axis,
                               float &min, float &max) {
    float proj0 = glm::dot(tri.v0, axis);
    float proj1 = glm::dot(tri.v1, axis);
    float proj2 = glm::dot(tri.v2, axis);

    min = std::min(proj0, std::min(proj1, proj2));
    max = std::max(proj0, std::max(proj1, proj2));
  }

  glm::vec3 CalculateContactPoint(const Triangle &triA, const Triangle &triB) {
    std::vector<glm::vec3> contacts;
    if (PointInTriangle(triA.v0, triB))
      contacts.push_back(triA.v0);
    if (PointInTriangle(triA.v1, triB))
      contacts.push_back(triA.v1);
    if (PointInTriangle(triA.v2, triB))
      contacts.push_back(triA.v2);
    if (PointInTriangle(triB.v0, triA))
      contacts.push_back(triB.v0);
    if (PointInTriangle(triB.v1, triA))
      contacts.push_back(triB.v1);
    if (PointInTriangle(triB.v2, triA))
      contacts.push_back(triB.v2);

    if (!contacts.empty()) {
      glm::vec3 avgContact(0.0f);
      for (const auto &contact : contacts) {
        avgContact += contact;
      }
      return avgContact / static_cast<float>(contacts.size());
    }

    glm::vec3 centroidA = (triA.v0 + triA.v1 + triA.v2) / 3.0f;
    glm::vec3 centroidB = (triB.v0 + triB.v1 + triB.v2) / 3.0f;
    return (centroidA + centroidB) / 2.0f;
  }

  bool PointInTriangle(const glm::vec3 &point, const Triangle &tri) {
    glm::vec3 v0 = tri.v2 - tri.v0;
    glm::vec3 v1 = tri.v1 - tri.v0;
    glm::vec3 v2 = point - tri.v0;

    float dot00 = glm::dot(v0, v0);
    float dot01 = glm::dot(v0, v1);
    float dot02 = glm::dot(v0, v2);
    float dot11 = glm::dot(v1, v1);
    float dot12 = glm::dot(v1, v2);

    float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return (u >= 0) && (v >= 0) && (u + v <= 1);
  }

public:
  MeshCollider() {}
  MeshCollider(const std::string &path, Transform *tf) {
    update(tf);
    LoadFromObj(path);
  }

  std::vector<Triangle> getTriangles() const { return triangles; }
  glm::mat4 getModel() const { return model; }

  void update(Transform *tf) {
    glm::mat3 rotationMat3(tf->getRight(), tf->getUp(), tf->getForward());
    glm::mat4 rotation = glm::mat4(rotationMat3);

    glm::mat4 translation = glm::translate(glm::mat4(1.0f), tf->getPosition());
    glm::mat4 scaling = glm::scale(glm::mat4(1.0f), tf->getScale());

    model = translation * rotation * scaling;
  }

  bool intersects(MeshCollider *other, Contact &out) {
    if (!CheckAABB(other)) {
      return false;
    }

    std::vector<Triangle> thisTriangles, otherTriangles;
    for (const auto &tri : triangles) {
      thisTriangles.push_back(TransformTriangle(tri, model));
    }
    for (const auto &tri : other->triangles) {
      otherTriangles.push_back(TransformTriangle(tri, other->model));
    }

    float minPenetration = std::numeric_limits<float>::max();
    Contact bestContact;
    bool hasCollision = false;

    const float MIN_PENETRATION_THRESHOLD = 0.001f;
    for (const auto &triA : thisTriangles) {
      for (const auto &triB : otherTriangles) {
        Contact contact;
        if (TriangleTriangleIntersect(triA, triB, contact)) {
          if (contact.penetration > MIN_PENETRATION_THRESHOLD) {
            hasCollision = true;
            if (contact.penetration < minPenetration) {
              minPenetration = contact.penetration;
              bestContact = contact;
            }
          }
        }
      }
    }

    if (hasCollision) {
      out = bestContact;
      return true;
    }
    return false;
  }
};