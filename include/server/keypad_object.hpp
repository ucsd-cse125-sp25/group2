#pragma once

#include "globals.hpp"
#include "server_gameobject.hpp"

class KeypadObject : public GameObject {
private:
  vector<int> correctSequence;

public:
  bool checkSequence(const vector<int> &inputSequence);
  KeypadObject(const OBJECT_ID id, const LevelType level, const bool isActive,
               glm::vec3 originalPosition, unique_ptr<Transform> &tf,
               unique_ptr<RigidBody> &rb, vector<Collider *> &cl,
               const vector<int> &sequence)
      : GameObject(id, level, isActive, originalPosition, tf, rb, cl),
        correctSequence(sequence), solved(false), locked(false),
        clientUsing(-1), opened(false){};
  bool solved;
  bool locked;
  CLIENT_ID clientUsing;
  bool opened;
};