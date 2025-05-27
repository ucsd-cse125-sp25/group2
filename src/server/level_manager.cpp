#include "level_manager.hpp"

void Level::addPuzzle(unique_ptr<Puzzle> puzzle) {
    puzzles.push_back(move(puzzle));
}

bool Level::isLevelComplete() const {
    for (const auto &puzzle : puzzles) {
        if (!puzzle->isPuzzleComplete()) {
            return false;
        }
    }
    return true;
}



