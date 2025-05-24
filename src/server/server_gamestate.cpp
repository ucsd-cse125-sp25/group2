#include "server_gamestate.hpp"

ServerGameState::ServerGameState() : deltaTime(0.007f) {
  physicsWorld = make_unique<Physics>();
}

bool ServerGameState::init() {
  ObjectLoader objectLoader = ObjectLoader();
  objectList = objectLoader.loadObjects();
  for (auto &obj : objectList)
    physicsWorld->add(obj.second.get());

  return true;
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
    updatedObjectIds.insert(id);
  }
}

void ServerGameState::updateRotation(int id, glm::vec3 rotation) {
  auto obj = getObject(id);
  if (obj) {
    obj->getTransform()->setRotation(rotation);
    updatedObjectIds.insert(id);
  }
}

void ServerGameState::updateInteraction(int id) {
  auto obj = getObject(id);
  if (obj) {
    cout << "Interacting with object: " << id << endl;
    updatedObjectIds.insert(id);
  }
}

void ServerGameState::applyPhysics() {
  physicsWorld->calculateForces();
  physicsWorld->moveObjects(deltaTime);
  physicsWorld->resolveCollisions();

  auto movedObjects = physicsWorld->getUpdatedObjects();
  for (auto id : movedObjects)
    updatedObjectIds.insert(id);
}

GameObject *ServerGameState::getObject(int id) {
  auto itr = objectList.find(id);
  if (itr != objectList.end()) {
    return itr->second.get();
  }
  cerr << "Object with id " << id << " not found" << endl;
  return nullptr;
}

std::vector<int> ServerGameState::getLastUpdatedObjects() {
  std::vector<int> list(updatedObjectIds.begin(), updatedObjectIds.end());
  updatedObjectIds.clear();
  return list;
}