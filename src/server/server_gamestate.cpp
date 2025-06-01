#include "server_gamestate.hpp"

ServerGameState::ServerGameState() : deltaTime(0.01f) {
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
  vector<OBJECT_ID> movedObjects;
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
      playerLogic->jump(player);
      break;
    case MovementType::GLIDE:
      playerLogic->glide(player);
      break;
    default:
      cerr << "Unknown movement type" << endl;
      break;
    }
    for (auto id : movedObjects)
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

void ServerGameState::updateInteraction(PLAYER_ID id) {
  GameObject *closestObject = nullptr;
  OBJECT_ID closestObjectID = -1;
  float minDistance = numeric_limits<float>::max();

  auto player = getObject(id);
  glm::vec3 rayOrigin =
      player->getCollider()[0]
          ->getCenter(); // use collider center instead since it is placed at
                         // the center of the player
  glm::vec3 rayDirection = glm::normalize(player->getTransform()->getForward());

  // Only need to iterate through the interactable objects
  for (auto &obj : interactableObjects) {
    auto object = obj.second;

    // If object is not active, it cannot be clicked on
    if (!object->isActive()) {
      continue;
    }
    glm::vec3 center = object->getTransform()->getPosition();
    glm::vec3 halfExtents = object->getCollider()[0]->getHalfExtents();

    glm::vec3 vDirToBox = center - rayOrigin;
    glm::vec3 vLineDir = glm::normalize(rayDirection);
    float t = glm::dot(vDirToBox, vLineDir);
    glm::vec3 closestPointOnRay;

    if (t <= 0.0f)
      closestPointOnRay = rayOrigin;
    else
      closestPointOnRay = rayOrigin + rayDirection * t;

    bool xInBox = (closestPointOnRay.x >= center.x - halfExtents.x &&
                   closestPointOnRay.x <= center.x + halfExtents.x);
    bool yInBox = (closestPointOnRay.y >= center.y - halfExtents.y &&
                   closestPointOnRay.y <= center.y + halfExtents.y);
    bool zInBox = (closestPointOnRay.z >= center.z - halfExtents.z &&
                   closestPointOnRay.z <= center.z + halfExtents.z);

    if (xInBox && yInBox && zInBox) {
      float distance = glm::distance(rayOrigin, closestPointOnRay);

      if (distance < minDistance) {
        closestObject = object;
        closestObjectID = obj.first;
        minDistance = distance;
      }
    }
  }

  cout << "Closest object: " << closestObjectID << endl;

  // If the character is holding an object, drop it
  GameObject *heldObject = playerLogic->getHeldObject(id);
  if (heldObject != nullptr) {
    playerLogic->dropObject(player, heldObject);
    cout << "Dropped object: " << heldObject->getId() << endl;
    playerLogic->setHeldObject(id, nullptr);
    cout << "Dropped object: " << closestObject->getId() << endl;
    updatedObjectIds.insert(closestObjectID);

  } else if (closestObjectID != -1) {

    // If interaction type is pickup and player is not holding an object
    if (closestObject->getInteractionType() == InteractionType::PICKUP &&
        heldObject == nullptr) {
      playerLogic->setHeldObject(id, closestObject);
      playerLogic->pickupObject(player, closestObject);
      cout << "Picked up object: " << closestObject->getId() << endl;
      updatedObjectIds.insert(closestObjectID);
      // If interaction type is press
    } else if (closestObject->getInteractionType() == InteractionType::PRESS) {
      closestObject->press();
      cout << "Pressed object: " << closestObjectID << endl;
    }

    if (closestObject->getInteractionType() == InteractionType::KEYPAD &&
        id == PIG) {
      auto keypadObject = dynamic_cast<KeypadObject *>(closestObject);
      if (keypadObject) {
        keypadObject->clientUsing = playerLogic->getClient(id);
        updateSpecialObjects.insert(closestObjectID);
      }
    }
    // If interaction type is note
    else if (closestObject->getInteractionType() == InteractionType::NOTE &&
        id == PIG) {
      auto noteObject = dynamic_cast<NoteObject *>(closestObject);
      if (noteObject) {
        noteObject->clientUsing = playerLogic->getClient(id);
        updateSpecialObjects.insert(closestObjectID);
      }
    }
  }
}

void ServerGameState::applyPhysics() {
  physicsWorld->calculateForces();
  physicsWorld->moveObjects(deltaTime);
  physicsWorld->resolveCollisions();

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

vector<int> ServerGameState::getLastUpdatedObjects() {
  vector<int> list(updatedObjectIds.begin(), updatedObjectIds.end());
  updatedObjectIds.clear();
  return list;
}

vector<int> ServerGameState::getSpecialUpdatedObjects() {
  vector<int> list(updateSpecialObjects.begin(), updateSpecialObjects.end());
  updateSpecialObjects.clear();
  return list;
}

bool ServerGameState::updateKeypadInput(OBJECT_ID id, vector<int> inputSequence,
                                        bool close) {
  auto keypadObject = dynamic_cast<KeypadObject *>(getObject(id));
  if (keypadObject) {
    if (close) {
      keypadObject->clientUsing = -1;
      keypadObject->opened = false;
      return true; // Doesn't matter the return type
    }
    if (keypadObject->checkSequence(inputSequence)) {
      cout << "Keypad ID: " << id << " unlocked!" << endl;
      return true;
    } else {
      cout << "Keypad ID: " << id << " Input: ";
      for (int i = 0; i < inputSequence.size(); ++i) {
        cout << inputSequence[i];
        cout << " ";
      }
      cout << " is incorrect." << endl;
    }
  } else {
    cerr << "KeypadObject with id " << id << " not found" << endl;
  }
  return false;
}