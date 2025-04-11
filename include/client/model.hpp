#pragma once

#include "core.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>
using namespace std;

class Model 
{
public:
    Model(const char *path);
    void Draw(const glm::mat4& viewProjMtx, Shader &shader);
    void Update();
private:
    vector<Mesh> meshes;
    vector<Texture> textures_loaded;
    string directory;

    void loadModel(string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
    unsigned int  TextureFromFile(const char *path, const string &directory);
};