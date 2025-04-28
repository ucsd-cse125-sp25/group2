#include "shared/core.hpp"
#include "client/model.hpp"
#include "shared/transform.hpp"
#include "shared/objects/cube.hpp"
#include <memory>
#include <string>

#ifndef OBJECT_HPP
#define OBJECT_HPP

using namespace std;

class GameObject {
protected:
  // shared properties
  int id;
  bool active;
  unique_ptr<Transform> transform;

  // client properties
  unique_ptr<Model> model;
  unique_ptr<Shader> shader;

  // server properties
  bool interactable;

public:
  GameObject(const int objectId, const bool isActive, unique_ptr<Transform>& tf) : id(objectId), active(isActive), transform(move(tf)) {
    model = nullptr;
    shader = nullptr;
    interactable = false;
  };

  virtual ~GameObject() {};

  // shared methods
  const int getId() const { return id; };
  bool isActive() const { return active; };
  glm::vec3 getPosition() {
    return transform->getPosition();
  };
  glm::vec3 getRotation() {return transform->getRotation();};
  glm::vec3 getScale() {return transform->getScale();};

  // client methods
  Model* getModel() const { return model.get(); };
  void setModel(unique_ptr<Model>& mod) { model = move(mod); };
  Shader* getShader() const { return shader.get(); };
  void setShader(unique_ptr<Shader>& shad) { shader = move(shad); };
  void Update(Transform *tf, float deltaTime);
  void Render(const glm::mat4 &viewProjMtx);

  // server methods
  void activate() { active = true; };
  void deactivate() { active = false; };
  void setInteractability(bool canInteract) {
    interactable = canInteract;
  };
  bool isInteractable() const { return interactable; };
};

#endif // OBJECT_HPP