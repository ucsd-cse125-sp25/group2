#pragma once

#include "client/camera.hpp"
#include "client/model.hpp"
#include "shader.hpp"
#include "shared/gameobject.hpp"
#include <unordered_map>
#include <vector>

class Renderer {
public:
  Renderer(Camera *camera);
  void Render(GameObject *o);

private:
  Camera *cam;
  std::unordered_map<GameObject *, Model *> renderObjects;
};
