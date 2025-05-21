#pragma once

#include "globals.hpp"
#include "server_gameobject.hpp"

#include <unordered_map>

using namespace std;

class GameLogic {
private:
  // Movement properties
  float speed;
  float jumpForce;

  // Interaction properties
  unordered_map<Characters, int> heldObjects;

public:
  GameLogic();

  void setHeldObject(Characters character, int objectID) {
    heldObjects[character] = objectID;
  };

    // Getters
    int getHeldObject(Characters character) {
        return heldObjects[character];
    };

    // Movement functions
    void moveObject(GameObject *object, glm::vec3 direction);
    void jump();

    // Ability functions
    void glide();
    
    // Interaction functions
    void pickupObject(GameObject *player,
                      GameObject *object);
    void dropObject(GameObject *player,
                    GameObject *object);
};