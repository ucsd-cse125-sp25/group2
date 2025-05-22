#include "server_gamestate.hpp"

ServerGameState::ServerGameState() : deltaTime(0.007f) {
  physicsWorld = make_unique<Physics>();
  playerLogic = make_unique<PlayerLogic>();
}

bool ServerGameState::init() {
  ObjectLoader objectLoader = ObjectLoader();
  objectList = objectLoader.loadObjects();
  for (auto &obj : objectList)
    physicsWorld->add(obj.second.get());

  return true;
}

CLIENT_ID *ServerGameState::updateCharacters(PLAYER_ID character,
                                             CLIENT_ID id) {
  playerLogic->assignCharacter(character, id);
  return playerLogic->getCharacterAssignments();
}

void ServerGameState::updateMovement(PLAYER_ID character, MovementType type,
                                     glm::vec3 cameraFront) {
  auto player = getObject(character);
  if (player) {
    // Find the direction of movement based on the camera's facing direction
    glm::vec3 flatFront =
        glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
    glm::vec3 cameraRight =
        glm::normalize(glm::cross(flatFront, glm::vec3(0.0f, 1.0f, 0.0f)));
    switch (type) {
    case MovementType::FORWARD:
      playerLogic->moveObject(player, flatFront);
      break;
    case MovementType::BACKWARD:
      playerLogic->moveObject(player, -flatFront);
      break;
    case MovementType::LEFT:
      playerLogic->moveObject(player, -cameraRight);
      break;
    case MovementType::RIGHT:
      playerLogic->moveObject(player, cameraRight);
      break;
    default:
      cerr << "Unknown movement type" << endl;
      break;
    }
    updatedObjectIds.insert(character);
  }
}

void ServerGameState::updateRotation(PLAYER_ID character, glm::vec3 rotation) {
  auto obj = getObject(character);
  if (obj) {
    obj->getTransform()->setRotation(rotation);
    updatedObjectIds.insert(character);
  }
}

void ServerGameState::updateInteraction(PLAYER_ID character,
                                        glm::vec3 rayDirection,
                                        glm::vec3 rayOrigin) {
  GameObject *closestObject = nullptr;
  OBJECT_ID closestObjectID;
  float minDistance = std::numeric_limits<float>::max();

  for (auto &obj : objectList) {
    auto object = obj.second.get();
    cout << "interaction type: "
         << static_cast<int>(object->getInteractionType())
         << endl; // delete later
    glm::vec3 center = object->getTransform()->getPosition();
    glm::vec3 halfExtents = object->getCollider()->getHalfExtents();

    glm::vec3 vDirToBox = center - rayOrigin;
    glm::vec3 vLineDir = glm::normalize(rayDirection);
    float fLineLength = 100.0f;
    float t = glm::dot(vDirToBox, vLineDir);
    glm::vec3 closestPointOnRay;

    if (t <= 0.0f)
      closestPointOnRay = rayOrigin;
    else if (t >= fLineLength)
      closestPointOnRay = rayOrigin + rayDirection;
    else
      closestPointOnRay = rayOrigin + rayDirection * t;

    glm::vec3 closestPointOnBox;

    closestPointOnBox.x =
        glm::clamp(closestPointOnRay.x, center.x - halfExtents.x,
                   center.x + halfExtents.x);
    closestPointOnBox.y =
        glm::clamp(closestPointOnRay.y, center.y - halfExtents.y,
                   center.y + halfExtents.y);
    closestPointOnBox.z =
        glm::clamp(closestPointOnRay.z, center.z - halfExtents.z,
                   center.z + halfExtents.z);

    float distance = glm::distance(closestPointOnRay, closestPointOnBox);

    if (closestObject == nullptr || distance < minDistance) {
      closestObject = object;
      closestObjectID = obj.first;
      minDistance = distance;
    }
  }

  cout << "Closest object: " << closestObjectID << endl; // delete later
  cout << "Interaction type: "
       << static_cast<int>(closestObject->getInteractionType())
       << endl; // delete later

  // if (closestObject->getInteractionType() == InteractionType::NONE) {
  //   cout << "No interaction available" << endl; // delete later
  //   return;
  // }

  // get player object and character
  auto player = getObject(character);

  if (closestObject->getInteractionType() == InteractionType::PICKUP ||
      closestObjectID == 1) {             // delete the OR later
    cout << "Pickup interaction" << endl; // delete later
    cout << "Held object id: " << playerLogic->getHeldObject(character)
         << endl; // delete later
    // If the character is already holding an object, drop it
    if (playerLogic->getHeldObject(character) == closestObjectID) {
      playerLogic->dropObject(player, closestObject);
      playerLogic->setHeldObject(character, -1);
      cout << "Dropped object: " << closestObject->getId() << endl;
    }
    // Chatecter is not holding an object, pick it up
    if (playerLogic->getHeldObject(character) == -1) {
      playerLogic->setHeldObject(character, closestObjectID);
      playerLogic->pickupObject(player, closestObject);
      cout << "Picked up object: " << closestObject->getId() << endl;
    }
  }
  updatedObjectIds.insert(closestObjectID);
}

void ServerGameState::applyPhysics() {
  physicsWorld->calculateForces();
  physicsWorld->resolveCollisions();
  physicsWorld->moveObjects(deltaTime);

  auto movedObjects = physicsWorld->getUpdatedObjects();
  for (auto id : movedObjects)
    updatedObjectIds.insert(id);
}

GameObject *ServerGameState::getObject(OBJECT_ID id) {
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