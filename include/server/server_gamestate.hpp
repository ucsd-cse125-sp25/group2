#pragma once

#include "csv.h"
#include "shared/gameobject.hpp"
#include "shared/objects/cube.hpp"
#include "shared/packets.hpp"

#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

using namespace std;

class ServerGameState {
public:
  ServerGameState();

  bool init();

  // update methods
  void updateMovement(int id, MovementType type, glm::vec3 cameraFront);

  // getters
  GameObject *getObject(int id);
  vector<int> getLastUpdatedObjects();

private:
  int level;
  unordered_map<int, unique_ptr<GameObject>> objectList;
  vector<int> updatedObjectIds;
};