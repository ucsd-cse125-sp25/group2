#pragma once

#include "base_object_loader.hpp"
#include "client_gameobject.hpp"
#include "model.hpp"
#include "shader.hpp"
#include "transform.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

class ObjectLoader : public BaseObjectLoader {
public:
  ObjectLoader() : BaseObjectLoader(){};
  unordered_map<int, unique_ptr<GameObject>> loadObjects();

private:
  int id = 0;
};