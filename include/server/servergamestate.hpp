#pragma once

#include "shared/gameobject.hpp"
#include "shared/packets.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class ServerGameState {
public:
  ServerGameState();

  unique_ptr<GameObject> createObject(vector<float> inputs);

  // update methods
  void updateMovement(int id, MovementType type);

  // getters
  GameObject *getObject(int id);
  vector<int> getLastUpdatedObjects();
  // void loadLevel(int new_level);

private:
  int level;
  unordered_map<int, unique_ptr<GameObject>> objectList;
  vector<int> updatedObjectIds;
};