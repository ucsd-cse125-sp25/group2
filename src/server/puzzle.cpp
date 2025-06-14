#include "puzzle.hpp"

void Puzzle::addCondition(unique_ptr<PuzzleCondition> condition) {
  conditions.push_back(move(condition));
}

bool Puzzle::isPuzzleComplete() const {
  for (const auto &condition : conditions) {
    if (!condition->isSatisfied()) {
      return false;
    }
  }
  return true;
}

pair<RewardType, vector<OBJECT_ID>> Puzzle::dispatchReward() {
  return make_pair(rewardType, objectIDs);
}