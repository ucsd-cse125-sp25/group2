#pragma once

#include "globals.hpp"
#include "server_gameobject.hpp"

class KeypadObject : public GameObject {
private:
  vector<int> correctSequence;

public:
  bool checkSequence(const vector<int> &inputSequence);
  KeypadObject(const OBJECT_ID id, const LEVEL_ID levelNum, const bool isActive,
               glm::vec3 originalPosition, unique_ptr<Transform> &tf,
               unique_ptr<RigidBody> &rb, vector<Collider *> &cl,
               const vector<int> &sequence)
      : GameObject(id, levelNum, isActive, originalPosition, tf, rb, cl),
        correctSequence(sequence), unlocked(false), locked(false),
        clientUsing(-1), opened(false){};
  bool unlocked;
  bool locked;
  CLIENT_ID clientUsing;
  bool opened;
};