#pragma once

#include "csv.h"
#include "packets.hpp"
#include "server_gameobject.hpp"

#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

using namespace std;

class ServerGameState {
private:
  int level;
  unordered_map<int, unique_ptr<GameObject>> objectList;
  vector<int> updatedObjectIds;

public:
  ServerGameState(){};

  bool init();

  // update methods
  void updateMovement(int id, MovementType type, glm::vec3 cameraFront);
  void updateInteraction(int id);

  // getters
  GameObject *getObject(int id);
  vector<int> getLastUpdatedObjects();
};