#pragma once

#include "globals.hpp"
#include "server_gameobject.hpp"


class KeypadObject : public GameObject {
    private:
        vector<int> correctSequence;
    public:
        bool checkSequence(const vector<int> &inputSequence);
        KeypadObject(const OBJECT_ID id, const bool isActive,
                     unique_ptr<Transform> &tf, unique_ptr<RigidBody> &rb,
                     unique_ptr<Collider> &cl, const vector<int> &sequence)
            : GameObject(id, isActive, tf, rb, cl), correctSequence(sequence), unlocked(false), locked(false), clientUsing(-1), opened(false) {};  
        bool unlocked;
        bool locked;
        CLIENT_ID clientUsing;
        bool opened;
};