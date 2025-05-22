#pragma once

#include "core.hpp"
#include "shader.hpp"

#include <memory>
#include <string>
#include <vector>

using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex {
  vec3 position;
  vec3 normal;
  vec2 texCoords;
  // Unneeded for now
  vec3 tangent;
  vec3 bitangent;
  int m_boneIDs[MAX_BONE_INFLUENCE];
  float m_weights[MAX_BONE_INFLUENCE];
};

struct Texture {
  unsigned int id;
  string type;
  string path;
};

struct MaterialColor {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
  MaterialColor()
      : ambient(0.2f, 0.2f, 0.2f), diffuse(0.8f, 0.8f, 0.8f),
        specular(0.0f, 0.0f, 0.0f), shininess(0.0f) {}
};

class Mesh {
private:
  unsigned int VAO, VBO, EBO;
  void setupMesh();

public:
  vector<Vertex> vertices;
  vector<unsigned int> indices;
  vector<Texture> textures;
  MaterialColor material;

  Mesh(vector<Vertex> vertices, vector<unsigned int> indices,
       vector<Texture> textures);
  Mesh(vector<Vertex> vertices, vector<unsigned int> indices,
       vector<Texture> textures, MaterialColor material);
  void draw(unique_ptr<Shader> &shader);
};