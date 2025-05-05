#include "server/servergamestate.hpp"

ServerGameState::ServerGameState() { loadLevel(0); }

unique_ptr<GameObject> ServerGameState::createObject(vector<float> inputs) {

  // TODO needs to be changed depending on how GameObject is changed.
  if (inputs.size() < 12) {
    cerr << "Unable to Create Object, input vector less than 12" << endl;
    return nullptr;
  }

  int id = int(inputs[0]);
  bool isActive = bool(int(inputs[2]));
  glm::vec3 position = glm::vec3(inputs[3], inputs[4], inputs[5]);
  glm::vec3 rotation = glm::vec3(inputs[6], inputs[7], inputs[8]);
  glm::vec3 scale = glm::vec3(inputs[9], inputs[10], inputs[11]);

  unique_ptr<Transform> tf = make_unique<Transform>(position, rotation, scale);
  unique_ptr<GameObject> new_obj = make_unique<GameObject>(id, isActive, tf);

  return new_obj;
}

GameObject* ServerGameState::getObject(int id) {
  auto itr = objects.find(id);
  if (itr != objects.end()) {
    return itr->second.get();
  }

  return nullptr;
}

vector<int> ServerGameState::getLastUpdatedObjects() {
  auto res = move(updated_ids);
  updated_ids.clear();
  return res;
}

void ServerGameState::updateObject(int id,
                                   unique_ptr<GameObject> updatedObject) {
  objects[id] = move(updatedObject);
  updated_ids.push_back(id);
}

void ServerGameState::loadLevel(int new_level) {
  level = new_level;
  ifstream object_file;
  objects.clear();
  updated_ids.clear();

  switch (new_level) {
  case 0:
    object_file.open("level0.csv");
    break;
  default:
    cerr << "No file for level " << new_level << endl;
    return;
  }

  if (object_file.is_open()) {
    string line;

    getline(object_file, line);

    while (getline(object_file, line)) {
      stringstream ss(line);
      string val;
      vector<float> inputs;

      while (getline(ss, val, ',')) {
        inputs.push_back(stof(val));
      }

      unique_ptr<GameObject> new_obj = createObject(inputs);
      int id = (*new_obj).getId();
      objects[id] = move(new_obj);
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