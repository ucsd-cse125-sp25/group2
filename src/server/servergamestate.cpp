#include "server/servergamestate.hpp"

ServerGameState::ServerGameState() {}

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

GameObject *ServerGameState::getObject(int id) {
  auto itr = objectList.find(id);
  if (itr != objectList.end()) {
    return itr->second.get();
  }
  cerr << "Object with id " << id << " not found" << endl;
  return nullptr;
}

vector<int> ServerGameState::getLastUpdatedObjects() {
  auto res = move(updatedObjectIds);
  updatedObjectIds.clear();
  return res;
}

void ServerGameState::updateMovement(int id, MovementType type) {
  auto obj = getObject(id);
  if (obj) {
    // do camera calculation here
    switch (type) {
      case MovementType::FORWARD:
        cout << "moving forward" << endl;
        // obj->move(vec/transform);
        break;
      case MovementType::BACKWARD:
        cout << "moving backward" << endl;
        // obj->move(vec/transform);
        break;
      case MovementType::LEFT:
        cout << "moving left" << endl;
        // obj->move(vec/transform);
        break;
      case MovementType::RIGHT:
        cout << "moving right" << endl;
        // obj->move(vec/transform);
        break;
      default:
        cerr << "Unknown movement type" << endl;
        break;
    }
    updatedObjectIds.push_back(id);
  }
}

/*
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
*/