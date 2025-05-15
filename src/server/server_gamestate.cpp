#include "server_gamestate.hpp"
#include "globals.hpp"

ServerGameState::ServerGameState() : deltaTime(0.007f) {
  physicsWorld = make_unique<Physics>();
}

bool ServerGameState::init() {
  // Initialize objects
  io::CSVReader<NUM_COLUMNS_CSV> in("../resources/objects/objects.csv");
  in.read_header(io::ignore_extra_column, "ID", "Active", "Px", "Py", "Pz",
                 "Rx", "Ry", "Rz", "Sx", "Sy", "Sz", "ModelPath",
                 "VertShaderPath", "FragShaderPath");

  int objectId;
  int isActive;
  float posX, posY, posZ;
  float rotX, rotY, rotZ;
  float scaleX, scaleY, scaleZ;
  string modelPath;
  string vertShaderPath, fragShaderPath;

  while (in.read_row(objectId, isActive, posX, posY, posZ, rotX, rotY, rotZ,
                     scaleX, scaleY, scaleZ, modelPath, fragShaderPath,
                     vertShaderPath)) {
    auto tf = make_unique<Transform>(glm::vec3(posX, posY, posZ),
                                     glm::vec3(rotX, rotY, rotZ),
                                     glm::vec3(scaleX, scaleY, scaleZ));
    auto rb = make_unique<RigidBody>();
    auto cl = make_unique<Collider>(tf->getPosition(), glm::vec3(1, 1, 1));
    // Make floor collider larger
    if (objectId == 2) {
      cl = make_unique<Collider>(glm::vec3(posX, posY, posZ),
                                 glm::vec3(10, 1, 10));
    }
    auto obj = make_unique<GameObject>(objectId, isActive, tf, rb, cl);
    // Make floor collider static
    if (objectId == 2) {
      obj->getRigidBody()->setStatic(true);
    }
    objectList[objectId] = move(obj);
    physicsWorld->add(objectList[objectId].get());
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
  auto player = getObject(id);
  if (player) {
    // Set player movement properties
    float speed = 10.0f;
    // Find the direction of movement based on the camera's facing direction
    glm::vec3 flatFront =
        glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
    glm::vec3 cameraRight =
        glm::normalize(glm::cross(flatFront, glm::vec3(0.0f, 1.0f, 0.0f)));
    switch (type) {
    case MovementType::FORWARD:
      player->getRigidBody()->applyImpulse(speed * flatFront);
      break;
    case MovementType::BACKWARD:
      player->getRigidBody()->applyImpulse(speed * -flatFront);
      break;
    case MovementType::LEFT:
      player->getRigidBody()->applyImpulse(speed * -cameraRight);
      break;
    case MovementType::RIGHT:
      player->getRigidBody()->applyImpulse(speed * cameraRight);
      break;
    default:
      cerr << "Unknown movement type" << endl;
      break;
    }
  }
}

void ServerGameState::updateInteraction(int id) {
  auto obj = getObject(id);
  if (obj) {
    cout << "Interacting with object: " << id << endl;
  }
}

void ServerGameState::applyPhysics() {
  physicsWorld->calculateForces();
  physicsWorld->resolveCollisions();
  vector<int> movedObjects = physicsWorld->moveObjects(deltaTime);

  for (auto id : movedObjects)
    updatedObjectIds.push_back(id);
}