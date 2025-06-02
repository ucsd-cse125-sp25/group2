#pragma once

#include "base_object_loader.hpp"
#include "globals.hpp"
#include "keypad_object.hpp"
#include "press_functions.hpp"
#include "server_gameobject.hpp"
#include "transform.hpp"

#include <fstream>
#include <iostream>
#include <magic_enum/magic_enum.hpp>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

class ObjectLoader : public BaseObjectLoader {
public:
  ObjectLoader() : BaseObjectLoader(){};
  unordered_map<OBJECT_ID, unique_ptr<GameObject>> loadObjects();
  unordered_set<OBJECT_ID> loadNotes();
};