#pragma once

#include "base_object_loader.hpp"
#include "globals.hpp"
#include "server_gameobject.hpp"
#include "transform.hpp"

#include <fstream>
#include <iostream>
#include <magic_enum.hpp>
#include <sstream>
#include <unordered_map>

class ObjectLoader : public BaseObjectLoader {
public:
  unordered_map<int, unique_ptr<GameObject>> loadObjects();

private:
  int id = 0;
};