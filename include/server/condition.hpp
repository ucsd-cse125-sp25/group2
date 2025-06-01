#pragma once

#include "globals.hpp"
#include "server_gameobject.hpp"

using namespace std;

enum class ConditionType { PRESSURE_PLATE, BUTTON, PIN };

class PuzzleCondition {
protected:
  GameObject *object;

public:
  PuzzleCondition(GameObject *obj) : object(obj) {}
  virtual bool isSatisfied() const = 0;
};

class PressurePlateCondition : public PuzzleCondition {
private:
  OBJECT_ID id;

public:
  PressurePlateCondition(GameObject *obj, OBJECT_ID objID)
      : PuzzleCondition(obj), id(objID) {}
  bool isSatisfied() const override;
};

class ButtonCondition : public PuzzleCondition {
public:
  ButtonCondition(GameObject *obj) : PuzzleCondition(obj) {}
  bool isSatisfied() const override;
};

class PinCondition : public PuzzleCondition {
public:
  PinCondition(GameObject *obj) : PuzzleCondition(obj) {}
  bool isSatisfied() const override;
};