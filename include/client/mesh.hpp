#pragma once

#include "client/shader.hpp"
#include "shared/core.hpp"
#include <memory>
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

struct MaterialColor {
  glm::vec3 ambient; 
  glm::vec3 diffuse;  
  glm::vec3 specular;  
  float shininess;     
  MaterialColor() : 
    ambient(0.2f, 0.2f, 0.2f),
    diffuse(0.8f, 0.8f, 0.8f),
    specular(0.0f, 0.0f, 0.0f),
    shininess(0.0f) {}
};

class Mesh {
public:
  vector<Vertex> vertices;
  vector<unsigned int> indices;
  vector<Texture> textures;
  MaterialColor material;

  Mesh(vector<Vertex> vertices, vector<unsigned int> indices,
       vector<Texture> textures);
  Mesh(vector<Vertex> vertices, vector<unsigned int> indices,
       vector<Texture> textures, MaterialColor material);
  void Draw(unique_ptr<Shader> &shader);

private:
  unsigned int VAO, VBO, EBO;
  void setupMesh();
};