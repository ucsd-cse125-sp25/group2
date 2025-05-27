#include "puzzle.hpp"

void Puzzle::addCondition(unique_ptr<PuzzleCondition> condition) {
    conditions.push_back(move(condition));
}

bool Puzzle::isPuzzleComplete() const {
    for (const auto &condition : conditions) {
        if (!condition->isSatified()) {
            return false;
        }
    }
    return true;
}