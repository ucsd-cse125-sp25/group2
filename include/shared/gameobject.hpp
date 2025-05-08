#pragma once

#include "client/model.hpp"
#include "shared/core.hpp"
#include "shared/transform.hpp"
#include "shared/types.hpp"

#include <memory>
#include <string>

using namespace std;

class GameObject {
protected:
  // shared properties
  int id;
  bool active;
  ObjectType type;
  unique_ptr<Transform> transform;

  // client properties
  unique_ptr<Model> model;
  unique_ptr<Shader> shader;

  // server properties
  bool interactable;

  // physics properties
  float velocity;

public:
  GameObject(const int objectId, const bool isActive, unique_ptr<Transform> &tf)
      : id(objectId), active(isActive), transform(move(tf)) {
    model = nullptr;
    shader = nullptr;
    interactable = false;
    velocity = 0.1f;
  };

  virtual ~GameObject(){};

  // shared methods
  const int getId() const { return id; };
  ObjectType getType() const { return type; };
  bool isActive() const { return active; };
  Transform *getTransform() { return transform.get(); };
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

  // physics methods
  void applyMovement(const glm::vec3 &direction);
};