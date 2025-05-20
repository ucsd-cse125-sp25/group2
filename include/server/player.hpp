#pragma once

#include "server_gameobject.hpp"
#include "globals.hpp"

class Player : public GameObject {
private:
    CLIENT_ID clientID;
    Characters character;
    bool isHolding;
    int heldObjectID;

public:
    Player(const int objectId, const bool isActive,
           unique_ptr<Transform> &tf, unique_ptr<RigidBody> &rb,
           unique_ptr<Collider> &cl)
        : GameObject(objectId, isActive, tf, rb, cl),
          isHolding(false), heldObjectID(-1) {}

    void setClientID(CLIENT_ID id) { clientID = id; }
    void setCharacter(Characters character) { this->character = character; }
    void setHolding(bool holding) { isHolding = holding; }
    void setHeldObject(int id) { heldObjectID = id; }

    CLIENT_ID getClientID() const { return clientID; }
    Characters getCharacter() const { return character; }
    bool isHoldingObject() const { return isHolding; }
    int getHeldObject() const { return heldObjectID; }
};