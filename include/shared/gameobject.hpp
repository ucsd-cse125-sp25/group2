#pragma once

#include "client/model.hpp"
#include "shared/core.hpp"
#include "shared/transform.hpp"

#include <memory>
#include <string>

using namespace std;

enum class ObjectType : uint8_t { CUBE };

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
  GameObject(const int objectId, const bool isActive, unique_ptr<Transform> &tf)
      : id(objectId), active(isActive), transform(move(tf)) {
    model = nullptr;
    shader = nullptr;
    interactable = false;
  };

  virtual ~GameObject(){};

  // shared methods
  const int getId() const { return id; };
  bool isActive() const { return active; };
  unique_ptr<Transform> getTransform() { return move(transform); };
  glm::vec3 getPosition() { return transform->getPosition(); };
  glm::vec3 getRotation() { return transform->getRotation(); };
  glm::vec3 getScale() { return transform->getScale(); };

  // client methods
  void setModel(unique_ptr<Model> mod) { model = move(mod); };
  void setShader(unique_ptr<Shader> shad) { shader = move(shad); };
  Model *getModel() const { return model.get(); };
  Shader *getShader() const { return shader.get(); };

  void update(Transform *tf);
  void draw(const glm::mat4 &viewProjMtx);

  // server methods
  void activate() { active = true; };
  void deactivate() { active = false; };
  void setInteractability(bool canInteract) { interactable = canInteract; };
  bool isInteractable() const { return interactable; };
};