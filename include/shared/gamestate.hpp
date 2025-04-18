#pragma once

#include "packets.hpp"
#include "cube.hpp"
#include <memory>
#include <vector>

class GameState {    
    public:
        GameState();
        PositionPacket init();
};