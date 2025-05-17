#pragma once

#include "packets.hpp"
#include "physics.hpp"
#include "server_gameobject.hpp"
#include "server_object_loader.hpp"

#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

using namespace std;

class ServerGameState {
private:
  int level;
  float deltaTime;
  unordered_map<int, unique_ptr<GameObject>> objectList;
  vector<int> updatedObjectIds;
  unique_ptr<Physics> physicsWorld;

public:
  ServerGameState();

  bool init();

  // update methods
  void updateMovement(int id, MovementType type, glm::vec3 cameraFront);
  void updateInteraction(int id);
  void applyPhysics();

  // getters
  GameObject *getObject(int id);
  vector<int> getLastUpdatedObjects();
};