#include "light_manager.hpp"

unordered_map<LEVEL_ID, vector<Light>> LightManager::lights;
LEVEL_ID LightManager::currentLevel = 0;

void LightManager::addLight(LEVEL_ID level, const glm::vec3 &pos,
                            const glm::vec3 &color, float radius,
                            bool useAtten) {
  if (level < NUM_LEVELS) {
    lights[level].push_back({pos, color, radius, useAtten});
  }
}

void LightManager::sendToShader(unique_ptr<Shader> &shader) {
  auto &currentLights = lights[currentLevel];
  int numLights = currentLights.size();

  shader->setInt("numLights", numLights);

  for (int i = 0; i < numLights; i++) {
    string base = "lightPositions[" + to_string(i) + "]";
    shader->setVec3(base, currentLights[i].position);

    base = "lightColors[" + to_string(i) + "]";
    shader->setVec3(base, currentLights[i].color);

    base = "lightRadii[" + to_string(i) + "]";
    shader->setFloat(base, currentLights[i].radius);

    base = "useAttenuation[" + to_string(i) + "]";
    shader->setBool(base, currentLights[i].useAttenuation);
  }
}

void LightManager::clearLights(LEVEL_ID level) { lights[level].clear(); }

void LightManager::setCurrentLevel(LEVEL_ID level) { currentLevel = level; }