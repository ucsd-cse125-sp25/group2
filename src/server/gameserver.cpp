#include "gameserver.hpp"

GameServer::GameServer(asio::io_context &io_context) {
  game = make_unique<ServerGameState>();
  network = make_unique<ServerNetwork>(io_context,
                                       loadConfig(CONFIG_PATH)["server-ip"],
                                       loadConfig(CONFIG_PATH)["port"]);
}

json GameServer::loadConfig(const string &path) {
  ifstream file(path);
  if (!file.is_open()) {
    throw runtime_error("Could not open config file at " + path);
  }
  json j;
  file >> j;
  return j;
}

GameServer::~GameServer() {}

bool GameServer::start() {
  if (!network->start()) {
    cerr << "ServerNetwork initialization failed" << endl;
    return false;
  }
  if (!game->init()) {
    cerr << "ServerGameState initialization failed" << endl;
    return false;
  }
  network->setOnJoin([game = game.get(), net = network.get()]() {
    if (game->state != Gamestate::GAME) {
      CharacterResponsePacket responsePacket(
          game->getPlayerLogic()->getCharacterAssignments());
      net->sendToAll(responsePacket);
    }
  });
  network->setOnLeave([game = game.get(), net = network.get()](CLIENT_ID id) {
    if (game->state != Gamestate::GAME) {
      game->getPlayerLogic()->unAssignCharacter(id);
      CharacterResponsePacket responsePacket(
          game->getPlayerLogic()->getCharacterAssignments());
      net->sendToAll(responsePacket);
    }
  });
  return true;
}

void GameServer::updateGameState() {
  deque<unique_ptr<IPacket>> packetsList = network->receiveFromClients();
  while (!packetsList.empty()) {
    unique_ptr<IPacket> packet = move(packetsList.front());
    packetsList.pop_front();

    switch (packet->getType()) {
    case PacketType::MOVEMENT: {
      auto movementPacket = static_cast<MovementPacket *>(packet.get());
      game->updateMovement(movementPacket->id, movementPacket->movementType);
      break;
    }
    case PacketType::ROTATION: {
      auto rotationPacket = static_cast<RotationPacket *>(packet.get());
      game->updateRotation(rotationPacket->id, rotationPacket->rotation);
      break;
    }
    case PacketType::INTERACTION: {
      auto interactionPacket = static_cast<InteractionPacket *>(packet.get());
      game->updateInteraction(interactionPacket->id);
      break;
    }
    case PacketType::CHARACTERSELECT: {
      auto characterPacket = static_cast<CharacterSelectPacket *>(packet.get());
      auto characterAssignments = game->updateCharacters(
          characterPacket->playerID, characterPacket->clientID);
      CharacterResponsePacket packet(characterAssignments);
      network->sendToAll(packet);
      // if (game->getPlayerLogic()->allCharactersAssigned()) {
      GameStatePacket statePacket(Gamestate::GAME);
      network->sendToAll(statePacket);
      // }
      break;
    }
    case PacketType::KEYPADINPUT: {
      auto keypadPacket = static_cast<KeypadInputPacket *>(packet.get());
      bool solved = game->updateKeypadInput(keypadPacket->objectID,
                                            keypadPacket->inputSequence,
                                            keypadPacket->close);
      if (!keypadPacket->close) {
        network->sendToClient(
            keypadPacket->clientID,
            KeypadPacket(keypadPacket->objectID, !solved, solved));
      }
      break;
    }
    case PacketType::SOUND: {
      auto soundPacket = static_cast<SoundPacket *>(packet.get());
      network->sendToAll(*soundPacket);
      break;
    }
    }
  }
  game->applyPhysics();
  triggerLevelChange = game->updateLevelManager();
}

void GameServer::dispatchUpdates() {
  vector<OBJECT_ID> updatedObjects = game->getLastUpdatedObjects();
  for (int i = 0; i < updatedObjects.size(); i++) {
    GameObject *obj = game->getObject(updatedObjects[i]);
    if (obj->getInteractionType() == InteractionType::KEYPAD) {
      auto keypadObject = dynamic_cast<KeypadObject *>(obj);
      if (keypadObject && keypadObject->clientUsing != -1 &&
          !keypadObject->opened) {
        network->sendToClient(
            keypadObject->clientUsing,
            KeypadPacket(keypadObject->getID(), true, keypadObject->solved));
        keypadObject->opened = true;
      }
    } else {
      ObjectPacket objPacket = ObjectPacket(
          obj->getID(),
          Transform(obj->getPosition(), obj->getRotation(), obj->getScale()),
          obj->isActive());
      network->sendToAll(objPacket);
    }
  }

  // If a puzzle is completed, send reward object
  vector<pair<RewardType, vector<OBJECT_ID>>> rewards =
      game->getRewardObjects();
  for (const auto &reward : rewards) {
    RewardType rewardType = reward.first;
    const vector<OBJECT_ID> &rewardObjectIDs = reward.second;

    if (rewardType == RewardType::ACTIVATE) {
      for (const OBJECT_ID &objectID : rewardObjectIDs) {
        game->getObject(objectID)->activate();
        ActivatePacket activatePacket(objectID);
        network->sendToAll(activatePacket);
      }
    } else if (rewardType == RewardType::DEACTIVATE) {
      for (const OBJECT_ID &objectID : rewardObjectIDs) {
        game->getObject(objectID)->deactivate();
        DeactivatePacket deactivatePacket(objectID);
        network->sendToAll(deactivatePacket);
      }
    }
  }

  // If a level is completed, notify all clients
  if (game->getPlayerLogic()->doNotifyClient()) {
    // Notify clients if pig picked up or dropped a note
    GameObject *heldObject = game->getPlayerLogic()->getHeldObject(PIG);
    OBJECT_ID heldNote = heldObject ? heldObject->getID() : -1;
    network->sendToClient(game->getPlayerLogic()->getClient(PIG),
                          NotePacket(heldNote));
  }

  if (triggerLevelChange) {
    triggerLevelChange = false;
    LevelChangePacket levelChangePacket(game->getLevel());
    network->sendToAll(levelChangePacket);
  }
}