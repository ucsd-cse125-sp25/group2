#pragma once

#include "globals.hpp"
#include "server_gameobject.hpp"

class NoteObject : public GameObject {
public:
  NoteObject(const OBJECT_ID id, const bool isActive,
               unique_ptr<Transform> &tf, unique_ptr<RigidBody> &rb,
               vector<Collider *> &cl)
      : GameObject(id, isActive, tf, rb, cl), clientUsing(-1), opened(false){};
  CLIENT_ID clientUsing;
  bool opened;
};