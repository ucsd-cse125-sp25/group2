#pragma once

#include "globals.hpp"
#include "server_gameobject.hpp"

#include <iostream>
#include <unordered_map>
#include <unordered_set>

using namespace std;

#define SHEEP_MAX_JUMPS 2

class PlayerLogic {
private:
  // Movement properties
  float speed;
  int sheepJumps;
  float jumpForce;
  float sheepJumpForce;
  float glideFallSpeed;
  bool sheepDoubleJump = false;

  // Interaction properties
  unordered_map<PLAYER_ID, GameObject *>
      heldObjects; // map from player ID to the object they are holding

  // Checking if pig has a note
  unordered_set<OBJECT_ID> pigNotes; // set of notes that the pig can hold
  bool pigNoteChange = false;

  // Client-to-Character mappings
  CLIENT_ID characterToClient[NUM_PLAYERS];
  PLAYER_ID clientToCharacter[NUM_PLAYERS];

public:
  PlayerLogic();

  // Setters
  void setHeldObject(PLAYER_ID id, GameObject *object) {
    heldObjects[id] = object;
  };
  void setNotes(unordered_set<OBJECT_ID> notes) { pigNotes = notes; };

  // Getters
  GameObject *getHeldObject(PLAYER_ID id) { return heldObjects[id]; };
  CLIENT_ID getClient(PLAYER_ID id) { return characterToClient[id]; };
  PLAYER_ID getCharacter(CLIENT_ID id) { return clientToCharacter[id]; };
  CLIENT_ID *getCharacterAssignments() { return characterToClient; }

  // Movement functions
  void move(GameObject *player, glm::vec3 direction);
  OBJECT_ID moveHeldObject(PLAYER_ID id, GameObject *player);
  void jump(GameObject *player);
  void rotate(GameObject *player, glm::vec3 rotation);
  void reset(GameObject *player);

  // Ability functions
  void glide(GameObject *chicken);
  bool doNotifyClient() {
    bool notify = pigNoteChange;
    pigNoteChange = false; // Reset the flag after notifying
    return notify;
  }

  // Interaction functions
  void pickupObject(GameObject *player, GameObject *object);
  void dropObject(GameObject *player, GameObject *object);

  // Character assignment functions
  void assignCharacter(PLAYER_ID id, CLIENT_ID clientID);
  void unAssignCharacter(CLIENT_ID id);
  bool allCharactersAssigned();
};