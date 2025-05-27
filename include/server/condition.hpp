#pragma once

#include "globals.hpp"
#include "server_gameobject.hpp"

using namespace std;

class PuzzleCondition {
public:
  virtual bool isSatisfied() const = 0;
};

class PressurePlateCondition : public PuzzleCondition {
private:
  GameObject *object;

public:
  PressurePlateCondition(GameObject *obj) : object(obj) {}
  bool isSatisfied() const override;
};

class ButtonCondition : public PuzzleCondition {
private:
  GameObject *object;

public:
  ButtonCondition(GameObject *obj) : object(obj) {}
  bool isSatisfied() const override;
};

class PinCondition : public PuzzleCondition {
private:
  GameObject *object;

public:
  PinCondition(GameObject *obj) : object(obj) {}
  bool isSatisfied() const override;
};