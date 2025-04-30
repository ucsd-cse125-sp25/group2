#include "client/model.hpp"
#include "shared/collider.hpp"
#include "shared/core.hpp"
#include "shared/objects/cube.hpp"
#include "shared/rigidbody.hpp"
#include "shared/transform.hpp"
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
  unique_ptr<RigidBody> rigidbody;
  unique_ptr<Collider> collider;

  // client properties
  glm::mat4 modelMatrix;
  unique_ptr<Model> model;
  unique_ptr<Shader> shader;

  // server properties
  bool interactable;

public:
  GameObject(const int objectId, const bool isActive, unique_ptr<Transform> &tf,
             unique_ptr<RigidBody> &rb, unique_ptr<Collider> &cl)
      : id(objectId), active(isActive), transform(move(tf)),
        rigidbody(move(rb)), collider(move(cl)) {
    model = nullptr;
    shader = nullptr;
    interactable = false;
  };

  virtual ~GameObject(){};

  // shared methods
  const int getId() const { return id; };
  bool isActive() const { return active; };
  Transform *getTransform() const { return transform.get(); }
  RigidBody *getRigidBody() const { return rigidbody.get(); }
  Collider *getCollider() const { return collider.get(); }

  // client methods
  Model *getModel() const { return model.get(); };
  void setModel(unique_ptr<Model> &mod) { model = move(mod); };
  Shader *getShader() const { return shader.get(); };
  void setShader(unique_ptr<Shader> &shad) { shader = move(shad); };
  void Update(float deltaTime);
  void Render(const glm::mat4 &viewProjMtx);

  // server methods
  void activate() { active = true; };
  void deactivate() { active = false; };
  void setInteractability(bool canInteract) { interactable = canInteract; };
  bool isInteractable() const { return interactable; };
};

#endif // OBJECT_HPP