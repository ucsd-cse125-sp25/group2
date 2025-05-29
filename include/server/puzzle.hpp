#pragma once

#include "condition.hpp"

#include <memory>
#include <vector>

using namespace std;

class Puzzle {
private:
  OBJECT_ID rewardObjectID;
  vector<unique_ptr<PuzzleCondition>> conditions;

public:
  Puzzle(OBJECT_ID id) : rewardObjectID(id) {};
  void addCondition(unique_ptr<PuzzleCondition> condition);
  bool isPuzzleComplete() const;
  OBJECT_ID dispatchReward();
};