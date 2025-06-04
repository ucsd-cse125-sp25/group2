#pragma once

#include "core.hpp"
#include "globals.hpp"
#include "shader.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct Light {
  glm::vec3 position;
  glm::vec3 color;
  float radius;
  bool useAttenuation;
};

class LightManager {
private:
  static unordered_map<LEVEL_ID, vector<Light>> lights;
  static LEVEL_ID currentLevel;

public:
  static void addLight(LEVEL_ID level, const glm::vec3 &pos,
                       const glm::vec3 &color, float radius,
                       bool useAtten = true);
  static void sendToShader(unique_ptr<Shader> &shader);
  static void clearLights(LEVEL_ID level);
  static void setCurrentLevel(LEVEL_ID level);
};