#include "server/server_gamestate.hpp"

ServerGameState::ServerGameState() {}

bool ServerGameState::init() {
  // Initialize objects
  io::CSVReader<15> in("../resources/objects/objects.csv");
  in.read_header(io::ignore_extra_column, "ID", "ObjectType", "Active", "Px",
                 "Py", "Pz", "Rx", "Ry", "Rz", "Sx", "Sy", "Sz", "ModelPath",
                 "VertShaderPath", "FragShaderPath");

  int objectId;
  string objectType;
  int isActive;
  float posX, posY, posZ;
  float rotX, rotY, rotZ;
  float scaleX, scaleY, scaleZ;
  string modelPath;
  string vertShaderPath, fragShaderPath;

  while (in.read_row(objectId, objectType, isActive, posX, posY, posZ, rotX,
                     rotY, rotZ, scaleX, scaleY, scaleZ, modelPath,
                     fragShaderPath, vertShaderPath)) {
    if (objectType == "CUBE") {
      auto tf = make_unique<Transform>(glm::vec3(posX, posY, posZ),
                                       glm::vec3(rotX, rotY, rotZ),
                                       glm::vec3(scaleX, scaleY, scaleZ));
      auto obj = make_unique<Cube>(objectId, isActive, tf);
      objectList[objectId] = move(obj);
    }
  }
  return true;
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
    auto tf = obj->getTransform();
    // do camera calculation here
    switch (type) {
    case MovementType::FORWARD:
      cout << "moving forward" << endl;
      tf->updatePosition(glm::vec3(0, 0, -1));
      // cout << obj->getPosition().x << " " << obj->getPosition().y << " " <<
      // obj->getPosition().z << endl;
      break;
    case MovementType::BACKWARD:
      cout << "moving backward" << endl;
      tf->updatePosition(glm::vec3(0, 0, 1));
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