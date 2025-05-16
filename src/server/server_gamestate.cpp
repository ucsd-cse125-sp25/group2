#include "server_gamestate.hpp"
#include "globals.hpp"
#include "server_object_loader.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;
ServerGameState::ServerGameState() : deltaTime(0.007f) {
  physicsWorld = make_unique<Physics>();
}

// For ServerGameState
bool ServerGameState::init() {
  ObjectLoader objectLoader = ObjectLoader();
  objectList = objectLoader.loadObjects();

  // if (obj->getInteractionType() != InteractionType::NONE)
  //     {
  //         interactableObjects[objData.id] = objectList[objData.id].get;
  //     }

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

void ServerGameState::updateInteraction(int id) {
  auto obj = getObject(id);
  if (obj) {
    cout << "Interacting with object: " << id << endl;
  }
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

void ServerGameState::applyPhysics() {
  physicsWorld->calculateForces();
  physicsWorld->resolveCollisions();
  physicsWorld->moveObjects(deltaTime);

  auto movedObjects = physicsWorld->getUpdatedObjects();
  for (auto id : movedObjects)
    updatedObjectIds.push_back(id);
}