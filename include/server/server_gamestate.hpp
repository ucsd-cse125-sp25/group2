#pragma once

#include "globals.hpp"
#include "packets.hpp"
#include "physics.hpp"
#include "player_logic.hpp"
#include "server_gameobject.hpp"
#include "server_object_loader.hpp"
#include "keypad_object.hpp"

#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

class ServerGameState {
private:
  int level;
  float deltaTime;
  unordered_map<int, unique_ptr<GameObject>> objectList;
  unordered_map<int, GameObject *> interactableObjects;
  unordered_set<int> updatedObjectIds;
  unique_ptr<Physics> physicsWorld;
  unique_ptr<PlayerLogic> playerLogic;

public:
  ServerGameState();

  bool init();
  Gamestate state;

  // update methods
  CLIENT_ID *updateCharacters(PLAYER_ID playerID, CLIENT_ID clientID);
  void updateMovement(PLAYER_ID id, MovementType type);
  void updateRotation(PLAYER_ID id, glm::vec3 rotation);
  void updateInteraction(PLAYER_ID id, glm::vec3 rayDirection,
                         glm::vec3 rayOrigin);
  void applyPhysics();
  bool updateKeypadInput(OBJECT_ID id, vector<int> inputSequence, bool close);

  // getters
  GameObject *getObject(OBJECT_ID id);
  vector<int> getLastUpdatedObjects();
  PlayerLogic *getPlayerLogic() { return playerLogic.get(); }
};