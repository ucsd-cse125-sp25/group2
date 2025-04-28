#include "server/servergamestate.hpp"


ServerGameState::ServerGameState() {
  level = 0;
  // read in file
  
  ifstream object_files("level0.txt");

  if (object_files.is_open()) {
    string line;

    getline(object_files, line);

    while (getline(object_files, line)) {
      
    }
    
  }

}

/*
Old Code
ObjectPacket GameState::init() {
  // initialize cube at the world origin
  x = 0.0f;
  y = 0.0f;
  z = 0.0f;
  ObjectPacket cube(0, ObjectType::CUBE, glm::vec3(x, y, z));
  return cube;
}

PositionPacket GameState::handleAction(ActionPacket *packet) {
  // Handle the action packet
  PositionPacket cube(
      0, glm::vec3(x, y, z)); // Declare and initialize cube before the switch
  switch (packet->type) {
  case ActionType::FORWARD:
    // Move forward
    z -= 1.0f;
    cube = PositionPacket(0, glm::vec3(x, y, z));
    return cube;
  case ActionType::BACK:
    // Move backward
    z += 1.0f;
    cube = PositionPacket(0, glm::vec3(x, y, z));
    break;
  case ActionType::LEFT:
    // Move left
    x -= 1.0f;
    cube = PositionPacket(0, glm::vec3(x, y, z));
    break;
  case ActionType::RIGHT:
    // Move right
    x += 1.0f;
    cube = PositionPacket(0, glm::vec3(x, y, z));
    break;
  default:
    break;
  }
  return cube; // Ensure cube is returned in all cases
}
*/