#pragma once

#include "globals.hpp"
#include "server_gameobject.hpp"

#include <unordered_map>

using namespace std;

class PlayerLogic {
private:
  // Movement properties
  float speed;
  float jumpForce;

  // Interaction properties
  unordered_map<PLAYER_ID, OBJECT_ID> heldObjects;

  // Client to character mapping
  CLIENT_ID characterToClient[NUM_PLAYERS];
  PLAYER_ID clientToCharacter[NUM_PLAYERS];

public:
  PlayerLogic();

  // Setters
  void setHeldObject(PLAYER_ID playerID, OBJECT_ID objectID) {
    heldObjects[playerID] = objectID;
  };

  // Getters
  OBJECT_ID getHeldObject(PLAYER_ID id) { return heldObjects[id]; };
  CLIENT_ID getClient(PLAYER_ID id) { return characterToClient[id]; };
  PLAYER_ID getCharacter(CLIENT_ID id) { return clientToCharacter[id]; };
  CLIENT_ID *getCharacterAssignments() { return characterToClient; }

  // Movement functions
  void move(PLAYER_ID id, GameObject *player, glm::vec3 direction);
  void jump();

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