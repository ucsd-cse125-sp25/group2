#pragma once

#include "base_gameobject.hpp"
#include "core.hpp"
#include "globals.hpp"
#include "model.hpp"
#include "transform.hpp"

#include <memory>
#include <string>

using namespace std;

class GameObject : public BaseGameObject {
protected:
  unique_ptr<Model> model;
  unique_ptr<Shader> shader;

public:
  GameObject(const OBJECT_ID id, const LEVEL_ID levelNum, const bool isActive,
             glm::vec3 originalPosition, unique_ptr<Transform> &tf)
      : BaseGameObject(id, levelNum, isActive, originalPosition, tf) {
    model = nullptr;
    shader = nullptr;
  };

  virtual ~GameObject(){};

  void setModel(unique_ptr<Model> mod) { model = move(mod); };
  void setShader(unique_ptr<Shader> shad) { shader = move(shad); };
  Model *getModel() const { return model.get(); };
  Shader *getShader() const { return shader.get(); };

  void update(Transform *tf);
  void draw(const glm::mat4 &viewProjMtx, const glm::vec3& pos);
};