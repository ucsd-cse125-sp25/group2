#pragma once

#include "base_gameobject.hpp"
#include "core.hpp"
#include "model.hpp"
#include "transform.hpp"
#include "globals.hpp"

#include <memory>
#include <string>

using namespace std;

class GameObject : public BaseGameObject {
protected:
  unique_ptr<Model> model;
  unique_ptr<Shader> shader;

public:
  GameObject(const int objectId, const bool isActive, unique_ptr<Transform> &tf)
      : BaseGameObject(objectId, isActive, tf) {
    model = nullptr;
    shader = nullptr;
  };

  virtual ~GameObject(){};

  void setModel(unique_ptr<Model> mod) { model = move(mod); };
  void setShader(unique_ptr<Shader> shad) { shader = move(shad); };
  Model *getModel() const { return model.get(); };
  Shader *getShader() const { return shader.get(); };

  void update(Transform *tf);
  void draw(const glm::mat4 &viewProjMtx);
};