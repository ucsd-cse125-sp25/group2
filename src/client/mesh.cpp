#include "mesh.hpp"

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices,
           vector<Texture> textures) {
  this->vertices = vertices;
  this->indices = indices;
  this->textures = textures;
  this->material = MaterialColor();

  setupMesh();
}

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices,
           vector<Texture> textures, MaterialColor material) {
  this->vertices = vertices;
  this->indices = indices;
  this->textures = textures;
  this->material = material;

  setupMesh();
}

void Mesh::setupMesh() {
  // Default OpenGL code to bind to VAO, VBO, and EBO
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0],
               GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               &indices[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, normal));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, texCoords));

  glBindVertexArray(0);
}

void Mesh::draw(unique_ptr<Shader> &shader) {
  shader->setVec3("materialAmbient", material.ambient);
  shader->setVec3("materialDiffuse", material.diffuse);
  shader->setVec3("materialSpecular", material.specular);
  shader->setFloat("materialShininess",
                   material.shininess > 0.0f ? material.shininess : 32.0f);

  bool hasTextures = !textures.empty();
  shader->setBool("useTexture", hasTextures);

  // Draw the mesh based on the texture
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  unsigned int normalNr = 1;
  unsigned int heightNr = 1;
  for (unsigned int i = 0; i < textures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 +
                    i); // active proper texture unit before binding
    // retrieve texture number (the N in diffuse_textureN)
    string number;
    string name = textures[i].type;
    if (name == "texture_diffuse")
      number = to_string(diffuseNr++);
    else if (name == "texture_specular")
      number = to_string(specularNr++); // transfer unsigned int to string
    else if (name == "texture_normal")
      number = to_string(normalNr++); // transfer unsigned int to string
    else if (name == "texture_height")
      number = to_string(heightNr++); // transfer unsigned int to string

    // now set the sampler to the correct texture unit
    glUniform1i(glGetUniformLocation(shader->ID, (name + number).c_str()), i);
    // and finally bind the texture
    glBindTexture(GL_TEXTURE_2D, textures[i].id);
  }

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()),
                 GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  glActiveTexture(GL_TEXTURE0);
}