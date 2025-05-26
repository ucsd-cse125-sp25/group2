#include "server_gamestate.hpp"

ServerGameState::ServerGameState() : deltaTime(0.007f) {
  physicsWorld = make_unique<Physics>();
  playerLogic = make_unique<PlayerLogic>();
}

bool ServerGameState::init() {
  ObjectLoader objectLoader = ObjectLoader();
  objectList = objectLoader.loadObjects();
  for (auto &obj : objectList) {
    auto object = obj.second.get();
    if (object->getInteractionType() != InteractionType::NONE) {
      interactableObjects[obj.first] = object;
    }
    physicsWorld->add(object);
  }

  return true;
}

CLIENT_ID *ServerGameState::updateCharacters(PLAYER_ID playerID,
                                             CLIENT_ID clientID) {
  playerLogic->assignCharacter(playerID, clientID);
  return playerLogic->getCharacterAssignments();
}

void ServerGameState::updateMovement(PLAYER_ID id, MovementType type) {
  auto player = getObject(id);
  if (player) {
    // Find the direction of movement based on the player's facing direction
    glm::vec3 playerFront = player->getTransform()->getForward();
    glm::vec3 flatFront =
        glm::normalize(glm::vec3(playerFront.x, 0.0f, playerFront.z));
    glm::vec3 playerRight =
        glm::normalize(glm::cross(flatFront, glm::vec3(0.0f, 1.0f, 0.0f)));
    switch (type) {
    case MovementType::FORWARD:
      playerLogic->move(player, flatFront);
      break;
    case MovementType::BACKWARD:
      playerLogic->move(player, -flatFront);
      break;
    case MovementType::LEFT:
      playerLogic->move(player, -playerRight);
      break;
    case MovementType::RIGHT:
      playerLogic->move(player, playerRight);
      break;
    case MovementType::JUMP:
      playerLogic->jump(player, deltaTime);
      break;
    case MovementType::GLIDE:
      playerLogic->glide(player, deltaTime);
      break;
    default:
      cerr << "Unknown movement type" << endl;
      break;
    }
    updatedObjectIds.insert(id);
  }
}

void ServerGameState::updateRotation(PLAYER_ID id, glm::vec3 rotation) {
  auto player = getObject(id);
  vector<OBJECT_ID> rotatedObjects;
  if (player) {
    rotatedObjects = playerLogic->rotate(id, player, rotation);
    updatedObjectIds.insert(rotatedObjects.begin(), rotatedObjects.end());
  }
}

void ServerGameState::updateInteraction(PLAYER_ID id, glm::vec3 rayDirection,
                                        glm::vec3 rayOrigin) {
  GameObject *closestObject = nullptr;
  OBJECT_ID closestObjectID = -1;
  float minDistance = numeric_limits<float>::max();

  // Only need to iterate through the interactable objects
  for (auto &obj : interactableObjects) {
    auto object = obj.second;

    // If object is not active, it cannot be clicked on
    if (!object->isActive()) {
      continue;
    }
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

  cout << "Closest object: " << closestObjectID << endl;

  auto player = getObject(id);

  // If the character is holding an object, drop it
  if (playerLogic->getHeldObject(id) != nullptr) {
    playerLogic->dropObject(player, closestObject);
    playerLogic->setHeldObject(id, nullptr);
  }

  // If an interactable object was clicked
  if (closestObjectID != -1) {
    // If interaction type is pickup and player is not holding an object
    if (closestObject->getInteractionType() == InteractionType::PICKUP &&
        playerLogic->getHeldObject(id) == nullptr) {
      playerLogic->setHeldObject(id, closestObject);
      playerLogic->pickupObject(player, closestObject);
      cout << "Picked up object: " << closestObject->getId() << endl;
      updatedObjectIds.insert(closestObjectID);
      // If interaction type is press
    } else if (closestObject->getInteractionType() == InteractionType::PRESS) {
      closestObject->press();
      cout << "Pressed object: " << closestObjectID << endl;
    }
  }
}

void ServerGameState::applyPhysics() {
  physicsWorld->calculateForces();
  physicsWorld->resolveCollisions();
  physicsWorld->moveObjects(deltaTime);

  auto movedObjects = physicsWorld->getUpdatedObjects();
  // if the object is held by a player, move it with the player
  for (auto id : movedObjects) {
    if (id < NUM_PLAYERS) {
      OBJECT_ID heldObjectId = playerLogic->moveHeldObject(id, getObject(id));
      if (heldObjectId != -1)
        updatedObjectIds.insert(heldObjectId);
    }
    updatedObjectIds.insert(id);
  }
}

GameObject *ServerGameState::getObject(OBJECT_ID id) {
  auto itr = objectList.find(id);
  if (itr != objectList.end()) {
    return itr->second.get();
  }
  cerr << "Object with ID " << id << " not found" << endl;
  return nullptr;
}

std::vector<int> ServerGameState::getLastUpdatedObjects() {
  std::vector<int> list(updatedObjectIds.begin(), updatedObjectIds.end());
  updatedObjectIds.clear();
  return list;
}