#pragma once

#include "core.hpp"
#include "globals.hpp"
#include "transform.hpp"

#include <magic_enum/magic_enum.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

using json = nlohmann::json;
using namespace std;

struct BaseObjectData {
  LevelType level;
  bool active;
  glm::vec3 originalPosition;
  unique_ptr<Transform> transform;
};

class BaseObjectLoader {
protected:
  OBJECT_ID id = 0;
  static glm::vec3 parseVec3(const json &json, const string &key,
                             const glm::vec3 &defaultValue);

public:
  BaseObjectData createBaseGameObject(const json &objData);
};
