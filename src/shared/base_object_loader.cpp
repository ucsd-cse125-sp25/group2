#include "base_object_loader.hpp"

BaseObjectData BaseObjectLoader::createBaseGameObject(const json &objData) {
  BaseObjectData baseObjectData;
  glm::vec3 position, rotation, scale;

  if (objData.contains("active")) {
    if (objData["active"].is_boolean()) {
      baseObjectData.active = objData["active"].get<bool>();
    } else if (objData["active"].is_number()) {
      baseObjectData.active = objData["active"].get<int>() != 0;
    } else {
      baseObjectData.active = true; // Default
    }
  } else {
    baseObjectData.active = true; // Default
  }

  // Parse transform data
  if (objData.contains("transform")) {
    auto &t = objData["transform"];
    position = parseVec3(t, "position", glm::vec3(0.0f));
    rotation = parseVec3(t, "rotation", glm::vec3(0.0f));
    scale = parseVec3(t, "scale", glm::vec3(1.0f));
  } else {
    position = glm::vec3(0.0f);
    rotation = glm::vec3(0.0f);
    scale = glm::vec3(1.0f);
  }

  baseObjectData.transform = make_unique<Transform>(position, rotation, scale);

  return baseObjectData;
}

glm::vec3 BaseObjectLoader::parseVec3(const json &json, const string &key,
                                 const glm::vec3 &defaultValue) {
  if (!json.contains(key) || !json[key].is_object()) {
    return defaultValue;
  }

  auto &vec = json[key];
  return glm::vec3(vec.value("x", defaultValue.x), vec.value("y", defaultValue.y),
              vec.value("z", defaultValue.z));
};