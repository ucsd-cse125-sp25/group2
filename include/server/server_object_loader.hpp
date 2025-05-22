#pragma once

#include "base_object_loader.hpp"
#include "globals.hpp"
#include "meshcollider.hpp"
#include "server_gameobject.hpp"
#include "transform.hpp"

#include <fstream>
#include <iostream>
#include <magic_enum/magic_enum.hpp>
#include <sstream>
#include <unordered_map>

class ObjectLoader : public BaseObjectLoader {
private:
  int id = 0;

public:
  unordered_map<int, unique_ptr<GameObject>> loadObjects();
  std::vector<Collider *> loadCollider(glm::vec3 objCenter, string path);
};