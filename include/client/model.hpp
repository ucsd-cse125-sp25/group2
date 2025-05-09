#pragma once

#include "core.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "transform.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <string>
#include <vector>

using namespace std;

class Model {
public:
  Model(const char *path);
  void draw(const glm::mat4 &viewProjMtx, unique_ptr<Shader> &shader);
  void update(Transform *transform);

private:
  glm::mat4 model;
  glm::vec3 color;
  vector<Mesh> meshes;
  vector<Texture> textures_loaded;
  string directory;

  void loadModel(string path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                       string typeName);
  unsigned int TextureFromFile(const char *path, const string &directory);
};