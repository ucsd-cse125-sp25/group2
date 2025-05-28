#pragma once

#include "condition.hpp"

#include <memory>
#include <vector>

using namespace std;

class Puzzle {
public:
  vector<unique_ptr<PuzzleCondition>> conditions;

  void addCondition(unique_ptr<PuzzleCondition> condition);
  bool isPuzzleComplete() const;
  void dispatchReward(GameObject *rewardObject);
};