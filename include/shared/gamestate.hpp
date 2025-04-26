#pragma once

#include "shared/packets.hpp"
#include "shared/cube.hpp"

#include <memory>
#include <vector>

class GameState {    
    private:
        float x, y, z;

    public:
        GameState();
        ObjectPacket init();
        PositionPacket handleAction(ActionPacket* packet);
};