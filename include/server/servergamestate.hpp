#pragma once

#include "shared/gameobject.hpp"
#include "shared/objects/cube.hpp"
#include "shared/packets.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class ServerGameState {
private:
  int level;
  unordered_map<string, GameObject> objects;
  vector<string> updated_ids;

  // float x, y, z;
  // some sort hard coding for objects at each level
  // perhaps this should be done in a txt file that is read in

public:
  ServerGameState();
  GameObject createObject(string id, ObjectType type);
  GameObject getObject(string id);
  vector<string> getLastUpdatedObjects();
  void updateObject(string id, GameObject updatedObject);
  void loadLevel(int new_level);

  /* Old Functions
  ObjectPacket init();
  PositionPacket handleAction(ActionPacket *packet);
  */
};