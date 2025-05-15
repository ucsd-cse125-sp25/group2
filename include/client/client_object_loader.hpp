#pragma once

#include "base_object_loader.hpp"
#include "transform.hpp"
#include "model.hpp"
#include "shader.hpp"
#include "client_gameobject.hpp"

#include <unordered_map>
#include <fstream>
#include <iostream>
#include <sstream>


class ObjectLoader : public BaseObjectLoader {
public:
  ObjectLoader() : BaseObjectLoader(){};
  unordered_map<int, unique_ptr<GameObject>> loadObjects();

private:
  int id = 0;
};