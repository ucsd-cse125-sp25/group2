#pragma once

#include "condition.hpp"

#include <memory>
#include <utility>
#include <vector>

using namespace std;

class Puzzle {
private:
  RewardType rewardType;
  vector<OBJECT_ID> objectIDs;
  vector<unique_ptr<PuzzleCondition>> conditions;

public:
  Puzzle(RewardType type, vector<OBJECT_ID> ids)
      : rewardType(type), objectIDs(ids) {}
  void addCondition(unique_ptr<PuzzleCondition> condition);
  bool isPuzzleComplete() const;
  pair<RewardType, vector<OBJECT_ID>> dispatchReward();
};