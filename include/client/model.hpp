#pragma once

#include "core.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "transform.hpp"
#include "light_manager.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stb_image.h>
#include <string>
#include <vector>

using namespace std;

class Model {
private:
  glm::vec3 color;
  vector<Mesh> meshes;
  vector<Texture> textures_loaded;
  string directory;

  void loadModel(string path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                       string typeName);
  unsigned int textureFromFile(const char *path, const string &directory);

public:
  bool isTransparent = false;
  glm::mat4 model;
  Model(const char *path);
  void draw(const glm::mat4 &viewProjMtx, const glm::vec3 &pos,
            unique_ptr<Shader> &shader);
  void update(Transform *transform);
  void changeColor(glm::vec3 col);
};