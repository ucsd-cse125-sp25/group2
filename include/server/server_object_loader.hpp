// object_loader.hpp
#pragma once

#include "base_object_loader.hpp"
#include "server_gameobject.hpp"
#include "transform.hpp"

#include <unordered_map>

using json = nlohmann::json;
using namespace std;

class ObjectLoader : public BaseObjectLoader {
public:
  unordered_map<int, unique_ptr<GameObject>> loadObjects();

private:
  int id = 0;
};