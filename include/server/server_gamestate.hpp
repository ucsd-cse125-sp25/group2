#pragma once

#include "globals.hpp"
#include "keypad_object.hpp"
#include "level_manager.hpp"
#include "packets.hpp"
#include "physics.hpp"
#include "player_logic.hpp"
#include "server_gameobject.hpp"
#include "server_object_loader.hpp"

#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

class ServerGameState {
private:
  float deltaTime;

  unordered_map<OBJECT_ID, unique_ptr<GameObject>> objectList;
  unordered_map<OBJECT_ID, GameObject *> interactableObjects;
  unordered_set<OBJECT_ID> updatedObjectIDs;
  vector<pair<RewardType, vector<OBJECT_ID>>> rewards;

  unique_ptr<Physics> physicsWorld;
  unique_ptr<PlayerLogic> playerLogic;
  unique_ptr<LevelManager> levelManager;

public:
  ServerGameState();

  bool init();
  Gamestate state;

  // update methods
  CLIENT_ID *updateCharacters(PLAYER_ID playerID, CLIENT_ID clientID);
  void updateMovement(PLAYER_ID id, MovementType type);
  void updateRotation(PLAYER_ID id, glm::vec3 rotation);
  void updateInteraction(PLAYER_ID id);
  bool updateKeypadInput(OBJECT_ID id, vector<int> inputSequence, bool close);
  bool updateLevelManager();
  void applyPhysics();

  // getters
  GameObject *getObject(OBJECT_ID id);
  vector<int> getLastUpdatedObjects();
  vector<pair<RewardType, vector<OBJECT_ID>>> getRewardObjects();
  PlayerLogic *getPlayerLogic() { return playerLogic.get(); }
  LevelManager *getLevelManager() { return levelManager.get(); }
  LevelType getLevel() { return levelManager->getLevel(); }
};