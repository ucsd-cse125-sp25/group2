#pragma once

#include "condition.hpp"

#include <memory>
#include <vector>

using namespace std;

class Puzzle {
private:
  GameObject *rewardObject;
  vector<unique_ptr<PuzzleCondition>> conditions;

public:
  Puzzle(GameObject *obj): rewardObject(obj) {};
  void addCondition(unique_ptr<PuzzleCondition> condition);
  bool isPuzzleComplete() const;
  OBJECT_ID dispatchReward();
};