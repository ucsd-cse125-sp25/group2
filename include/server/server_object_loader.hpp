#pragma once

#include "base_object_loader.hpp"
#include "globals.hpp"
#include "press_functions.hpp"
#include "server_gameobject.hpp"
#include "keypad_object.hpp"
#include "transform.hpp"

#include <fstream>
#include <iostream>
#include <magic_enum/magic_enum.hpp>
#include <sstream>
#include <unordered_map>

class ObjectLoader : public BaseObjectLoader {
public:
  ObjectLoader() : BaseObjectLoader(){};
  unordered_map<int, unique_ptr<GameObject>> loadObjects();
  vector<Collider *> loadCollider(string path);
};