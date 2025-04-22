#pragma once

#include "shared/packets.hpp"
#include "shared/cube.hpp"

#include <memory>
#include <vector>

class GameState {    
    public:
        GameState();
        PositionPacket init();
};