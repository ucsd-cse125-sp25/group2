#include "base_object_loader.hpp"

BaseObjectData BaseObjectLoader::createBaseGameObject(const json &objData) {
  BaseObjectData baseObjectData;
  vec3 position, rotation, scale;

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
    position = parseVec3(t, "position", vec3(0.0f));
    rotation = parseVec3(t, "rotation", vec3(0.0f));
    scale = parseVec3(t, "scale", vec3(1.0f));
  } else {
    position = vec3(0.0f);
    rotation = vec3(0.0f);
    scale = vec3(1.0f);
  }

  baseObjectData.transform = make_unique<Transform>(position, rotation, scale);

  return baseObjectData;
}

vec3 BaseObjectLoader::parseVec3(const json &json, const string &key,
                                 const vec3 &defaultValue) {
  if (!json.contains(key) || !json[key].is_object()) {
    return defaultValue;
  }

  auto &vec = json[key];
  return vec3(vec.value("x", defaultValue.x), vec.value("y", defaultValue.y),
              vec.value("z", defaultValue.z));
};