#include "light_manager.hpp"

unordered_map<LevelType, vector<Light>> LightManager::lights;
LevelType LightManager::currentLevel = LevelType::BARN;

void LightManager::addLight(LevelType level, const glm::vec3 &pos,
                            const glm::vec3 &color, float radius,
                            bool useAtten) {
  lights[level].push_back({pos, color, radius, useAtten});
}

void LightManager::sendToShader(unique_ptr<Shader> &shader) {
  auto &currentLights = lights[currentLevel];
  int numLights = currentLights.size();
  // cout << "Sending " << numLights << " lights to shader for level "
  //      << static_cast<int>(currentLevel) << endl;

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

void LightManager::clearLights(LevelType level) { lights[level].clear(); }

void LightManager::setCurrentLevel(LevelType level) { currentLevel = level; }