#include "shared/gamestate.hpp"

GameState::GameState() {}

PositionPacket GameState::init() {
    //initialize cube at the world origin
    PositionPacket cube(0.0f, 0.0f, 0.0f);
    return cube;
}