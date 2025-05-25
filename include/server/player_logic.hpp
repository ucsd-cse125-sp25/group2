#pragma once

#include "globals.hpp"
#include "server_gameobject.hpp"

#include <iostream>
#include <unordered_map>

using namespace std;

class PlayerLogic {
private:
  // Movement properties
  float speed;
  float jumpForce;
  float jumpTime;
  float maxJumpTime;

  // Interaction properties
  unordered_map<PLAYER_ID, GameObject *>
      heldObjects; // map from player ID to the object they are holding

  // Client-to-Character mappings
  CLIENT_ID characterToClient[NUM_PLAYERS];
  PLAYER_ID clientToCharacter[NUM_PLAYERS];

public:
  PlayerLogic();

  // Setters
  void setHeldObject(PLAYER_ID id, GameObject *object) {
    heldObjects[id] = object;
  };

  // Getters
  GameObject *getHeldObject(PLAYER_ID id) { return heldObjects[id]; };
  CLIENT_ID getClient(PLAYER_ID id) { return characterToClient[id]; };
  PLAYER_ID getCharacter(CLIENT_ID id) { return clientToCharacter[id]; };
  CLIENT_ID *getCharacterAssignments() { return characterToClient; }

  // Movement functions
  vector<OBJECT_ID> jump(PLAYER_ID id, GameObject *player, float deltaTime);
  void move(GameObject *player, glm::vec3 direction);
  OBJECT_ID moveHeldObject(PLAYER_ID id, GameObject *player);
  vector<OBJECT_ID> rotate(PLAYER_ID id, GameObject *player,
                           glm::vec3 rotation);

  // Ability functions
  void glide();

  // Interaction functions
  void pickupObject(GameObject *player, GameObject *object);
  void dropObject(GameObject *player, GameObject *object);

  // Character assignment functions
  void assignCharacter(PLAYER_ID id, CLIENT_ID clientID);
  void unAssignCharacter(CLIENT_ID id);
  bool allCharactersAssigned();
};