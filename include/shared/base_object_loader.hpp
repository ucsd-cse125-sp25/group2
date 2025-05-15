#pragma once

#include "core.hpp"
#include "transform.hpp"

#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

using json = nlohmann::json;
using namespace std;

struct BaseObjectData {
  bool active;
  unique_ptr<Transform> transform;
};

class BaseObjectLoader {
public:
  BaseObjectData createBaseGameObject(const json &objData);

private:
  static glm::vec3 parseVec3(const json &json, const string &key,
                        const glm::vec3 &defaultValue);
};
