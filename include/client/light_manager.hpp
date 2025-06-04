#pragma once

#include "core.hpp"
#include "shader.hpp"
#include "globals.hpp"

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <iostream>

using namespace std;

struct Light {
    glm::vec3 position;
    glm::vec3 color;
    float radius;
    bool useAttenuation;
};

static unordered_map<LEVEL_ID, vector<Light>> lights;
static LEVEL_ID currentLevel;

class LightManager {
public:
    static void setCurrentLevel(LEVEL_ID level) {
        currentLevel = level;
    }
    
    static void addLight(LEVEL_ID level, const glm::vec3& pos, const glm::vec3& color, float radius, bool useAtten = true) {
        if (level < NUM_LEVELS) {
            lights[level].push_back({pos, color, radius, useAtten});
        }
    }
    
    static void clearLights() {
        lights.clear();
    }
    
    static void sendToShader(unique_ptr<Shader>& shader) {
        auto currentLights = lights[currentLevel];
        int numLights = static_cast<int>(currentLights.size());
        
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
};