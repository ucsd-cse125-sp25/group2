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
  static unordered_map<LevelType, vector<Light>> lights;
  static LevelType currentLevel;

public:
  static void addLight(LevelType level, const glm::vec3 &pos,
                       const glm::vec3 &color, float radius,
                       bool useAtten = true);
  static void sendToShader(unique_ptr<Shader> &shader);
  static void clearLights(LevelType level);
  static void setCurrentLevel(LevelType level);
};