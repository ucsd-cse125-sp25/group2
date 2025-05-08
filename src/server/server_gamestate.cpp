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

void ServerGameState::updateMovement(int id, MovementType type, 
                                     glm::vec3 cameraFront) {
  auto obj = getObject(id);
  if (obj) {
    // Find the direction of movement based on the camera's facing direction
    glm::vec3 flatFront = glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
    glm::vec3 cameraRight = glm::normalize(glm::cross(flatFront, glm::vec3(0.0f, 1.0f, 0.0f)));
    switch (type) {
    case MovementType::FORWARD:
      obj->applyMovement(flatFront);
      break;
    case MovementType::BACKWARD:
      obj->applyMovement(-flatFront);
      break;
    case MovementType::LEFT:
      obj->applyMovement(-cameraRight);
      break;
    case MovementType::RIGHT:
      obj->applyMovement(cameraRight);
      break;
    default:
      cerr << "Unknown movement type" << endl;
      break;
    }
    updatedObjectIds.push_back(id);
  }
}