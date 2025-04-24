#include "shared/gamestate.hpp"

GameState::GameState() {}

PositionPacket GameState::init() {
    //initialize cube at the world origin
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    PositionPacket cube(x, y, z);
    return cube;
}

PositionPacket GameState::handleAction(ActionPacket* packet) {
    // Handle the action packet
    PositionPacket cube(x, y, z); // Declare and initialize cube before the switch
    switch (packet->type) {
        case ActionType::FORWARD:
            // Move forward
            z++;
            cube = PositionPacket(x, y, z);
            return cube;
        case ActionType::BACK:
            // Move backward
            break;
        case ActionType::LEFT:
            // Move left
            break;
        case ActionType::RIGHT:
            // Move right
            break;
        default:
            break;
    }
    return cube; // Ensure cube is returned in all cases
}