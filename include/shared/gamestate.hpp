#pragma once

#include "shared/objects/cube.hpp"
#include "shared/packets.hpp"

#include <memory>
#include <vector>

class GameState {
private:
  float x, y, z;

public:
  GameState();
  ObjectPacket init();
  PositionPacket handleAction(ActionPacket *packet);
};