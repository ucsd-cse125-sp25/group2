#pragma once

#include "client/shader.hpp"
#include "core.hpp"
#include <string>
#include <vector>
using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
  // Unneeded for now
  glm::vec3 tangent;
  glm::vec3 bitangent;
  int m_boneIDs[MAX_BONE_INFLUENCE];
  float m_weights[MAX_BONE_INFLUENCE];
};

struct Texture {
  unsigned int id;
  string type;
  string path;
};

class Mesh {
public:
  vector<Vertex> vertices;
  vector<unsigned int> indices;
  vector<Texture> textures;

  Mesh(vector<Vertex> vertices, vector<unsigned int> indices,
       vector<Texture> textures);
  void Draw(Shader &shader);

private:
  unsigned int VAO, VBO, EBO;
  void setupMesh();
};