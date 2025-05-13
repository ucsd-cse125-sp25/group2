#include "model.hpp"

Model::Model(const char *path) {
  model = glm::mat4(1);
  color = glm::vec3(1.0f, 0.95f, 0.1f);
  loadModel(path);
}

void Model::changeColor(glm::vec3 col) { this->color = col; }

void Model::draw(const glm::mat4 &viewProjMtx, unique_ptr<Shader> &shader) {
  // Activate the shader program
  shader->use();
  // Send camera view projection matrix to vertex shader file
  shader->setMat4("viewProj", viewProjMtx);
  // Send model matrix to vertex shader file
  shader->setMat4("model", model);
  shader->setVec3("DiffuseColor", color);
  for (unsigned int i = 0; i < meshes.size(); i++)
    // Draw each mesh
    meshes[i].draw(shader);
}

void Model::update(Transform *transform) {
  glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), transform->getScale());
  glm::mat4 rotMat =
      glm::rotate(glm::mat4(1.0f), glm::radians(transform->getRotation().x),
                  glm::vec3(1, 0, 0));
  rotMat = glm::rotate(rotMat, glm::radians(transform->getRotation().y),
                       glm::vec3(0, 1, 0));
  rotMat = glm::rotate(rotMat, glm::radians(transform->getRotation().z),
                       glm::vec3(0, 0, 1));
  glm::mat4 transMat =
      glm::translate(glm::mat4(1.0f), transform->getPosition());

  model = transMat * rotMat * scaleMat;
}

void Model::loadModel(string path) {
  Assimp::Importer import;
  const aiScene *scene =
      import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
    return;
  }
  directory = path.substr(0, path.find_last_of('/'));

  // process ASSIMP's root node recursively
  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
  // Process every mesh in this node
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene));
  }
  // Process each of this node's children
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene);
  }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
  vector<Vertex> vertices;
  vector<unsigned int> indices;
  vector<Texture> textures;
  MaterialColor material;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;
    glm::vec3 vector;
    // Positions
    vector.x = mesh->mVertices[i].x;
    vector.y = mesh->mVertices[i].y;
    vector.z = mesh->mVertices[i].z;
    vertex.position = vector;

    // Normals
    if (mesh->HasNormals()) {
      vector.x = mesh->mNormals[i].x;
      vector.y = mesh->mNormals[i].y;
      vector.z = mesh->mNormals[i].z;
      vertex.normal = vector;
    }

    // Texture Coordinates
    if (mesh->mTextureCoords[0]) {
      glm::vec2 vec;
      vec.x = mesh->mTextureCoords[0][i].x;
      vec.y = mesh->mTextureCoords[0][i].y;
      vertex.texCoords = vec;
    } else
      vertex.texCoords = glm::vec2(0);

    // Tangents and Bitangents
    if (mesh->HasTangentsAndBitangents()) {
      vector.x = mesh->mTangents[i].x;
      vector.y = mesh->mTangents[i].y;
      vector.z = mesh->mTangents[i].z;
      vertex.tangent = vector;
      vector.x = mesh->mBitangents[i].x;
      vector.y = mesh->mBitangents[i].y;
      vector.z = mesh->mBitangents[i].z;
      vertex.bitangent = vector;
    }
    vertices.push_back(vertex);
  }
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    // Get each mesh's face and for each face get the indices
    for (unsigned int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }
  if (mesh->mMaterialIndex >= 0) {
    // Object comes with diffuse, normal, roughness, and specular maps, so we
    // can insert them as textures

    aiMaterial *aiMat = scene->mMaterials[mesh->mMaterialIndex];

    aiColor3D diffuse(0, 0, 0);
    aiColor3D ambient(0, 0, 0);
    aiColor3D specular(0, 0, 0);
    float shininess = 0;

    aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    aiMat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    aiMat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
    aiMat->Get(AI_MATKEY_SHININESS, shininess);

    material.diffuse = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
    material.ambient = glm::vec3(ambient.r, ambient.g, ambient.b);
    material.specular = glm::vec3(specular.r, specular.g, specular.b);
    material.shininess = shininess;

    vector<Texture> diffuseMaps =
        loadMaterialTextures(aiMat, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    vector<Texture> specularMaps =
        loadMaterialTextures(aiMat, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    vector<Texture> normalMaps =
        loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    vector<Texture> heightMaps =
        loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
  }
  return Mesh(vertices, indices, textures, material);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                            string typeName) {
  vector<Texture> textures;
  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
    aiString path;
    mat->GetTexture(type, i, &path);
    bool loaded = false;
    // Check if we have already loaded this texture before, otherwise add it to
    // textures_loaded for next time Optimizes so we aren't reloading textures
    for (unsigned int j = 0; j < textures_loaded.size(); j++) {
      if (strcmp(textures_loaded[j].path.data(), path.C_Str()) == 0) {
        textures.push_back(textures_loaded[j]);
        loaded = true;
        break;
      }
    }
    if (!loaded) {
      Texture texture;
      texture.id = textureFromFile(path.C_Str(), directory);
      texture.type = typeName;
      texture.path = path.C_Str();
      textures.push_back(texture);
      textures_loaded.push_back(texture);
    }
  }
  return textures;
}

// Generates a texture from the image provided.
unsigned int Model::textureFromFile(const char *path, const string &directory) {
  string filename = string(path);
  filename = directory + '/' + filename;

  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char *data =
      stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
  if (data) {
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  } else {
    cout << "Texture failed to load at path: " << path << endl;
    stbi_image_free(data);
  }
  return textureID;
}