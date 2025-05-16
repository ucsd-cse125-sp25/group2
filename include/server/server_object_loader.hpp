#pragma once

#include "base_object_loader.hpp"
#include "server_gameobject.hpp"
#include "transform.hpp"
#include "globals.hpp"

#include <unordered_map>
#include <fstream>
#include <iostream>
#include <magic_enum/magic_enum.hpp>
#include <sstream>

class ObjectLoader : public BaseObjectLoader {
public:
  unordered_map<int, unique_ptr<GameObject>> loadObjects();

private:
  int id = 0;
};