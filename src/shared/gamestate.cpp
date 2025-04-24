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

PositionPacket GameState::handleAction(ActionPacket packet) {
    // Handle the action packet
    switch (packet.action) {
        case ActionType::FORWARD:
            // Move forward
            z++;
            PositionPacket cube(x, y, z);
            return cube;
            break;
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
}