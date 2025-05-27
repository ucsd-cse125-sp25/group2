#pragma once

#include "condition.hpp"

#include <vector>
#include <memory>

using namespace std;

class Puzzle {
public:
    vector<unique_ptr<PuzzleCondition>> conditions;

    void addCondition(unique_ptr<PuzzleCondition> condition);
    bool isPuzzleComplete() const;
};