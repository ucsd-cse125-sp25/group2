#pragma once

#include "shared/gameobject.hpp"
#include "shared/objects/cube.hpp"
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
private:
  int level;
  unordered_map<int, unique_ptr<GameObject>> objects;
  vector<int> updated_ids;

  // float x, y, z;
  // some sort hard coding for objects at each level
  // perhaps this should be done in a txt file that is read in

public:
  ServerGameState();
  unique_ptr<GameObject> createObject(vector<float> inputs);
  unique_ptr<GameObject> getObject(int id);
  vector<int> getLastUpdatedObjects();
  void updateObject(int id, unique_ptr<GameObject> updatedObject);
  void loadLevel(int new_level);

  /* Old Functions
  ObjectPacket init();
  PositionPacket handleAction(ActionPacket *packet);
  */
};