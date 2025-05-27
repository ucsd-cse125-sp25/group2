#pragma once 

#include "globals.hpp"
#include "server_gameobject.hpp"

using namespace std;

class PuzzleCondition {
public:
    virtual bool isSatified() const = 0;
};

class PressurePlateCondition : public PuzzleCondition {
private:
    GameObject *object;
public:
    PressurePlateCondition(GameObject *obj) : object(obj) {}
    bool isSatified() const override;
};