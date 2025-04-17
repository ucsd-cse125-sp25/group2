#pragma once

#include "packets.hpp"
#include "cube.hpp"
#include <memory>
#include <vector>

class GameState {
    private:
    std::vector<IPacket> initial_objects;
    
    public:
        GameState();
        PositionPacket init();
};