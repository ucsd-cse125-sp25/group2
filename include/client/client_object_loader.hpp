#pragma once

#include "base_object_loader.hpp"
#include "client_gameobject.hpp"
#include "light_manager.hpp"
#include "model.hpp"
#include "shader.hpp"
#include "transform.hpp"
#include "ui.hpp"
#include "ui_manager.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

class ObjectLoader : public BaseObjectLoader {
public:
  ObjectLoader() : BaseObjectLoader(){};
  unordered_map<OBJECT_ID, unique_ptr<GameObject>> loadObjects();
  unordered_map<OBJECT_ID, unique_ptr<BaseUI>> loadNotes();
  // LevelType getLevelFromString(const std::string &levelStr);
  void loadLights();
};