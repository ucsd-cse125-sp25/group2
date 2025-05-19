#include "gameserver.hpp"

GameServer::GameServer(asio::io_context &io_context, const string &ip,
                       const string &port) {
  game = make_unique<ServerGameState>();
  network = make_unique<ServerNetwork>(io_context, ip, port);
  characterManager = make_unique<CharacterManager>();
}

GameServer::~GameServer() {}

void GameServer::start() {
  network->start();
  if (!game->init()) {
    cerr << "ServerGameState initialization failed" << endl;
    return;
  }
  network->setOnJoin(
      [game = game.get(), &select = characterManager, net = network.get()]() {
        if (game->state != Gamestate::GAME) {
          CharacterResponsePacket responsePacket(select->chicken, select->sheep,
                                                 select->pig, select->cow);
          net->sendToAll(responsePacket);
        }
      });
  network->setOnLeave([game = game.get(), &select = characterManager,
                       net = network.get()](int id) {
    if (game->state != Gamestate::GAME) {
      select->isAlreadyAssigned(id);
      CharacterResponsePacket responsePacket(select->chicken, select->sheep,
                                             select->pig, select->cow);
      net->sendToAll(responsePacket);
    }
  });
}

void GameServer::updateGameState() {
  deque<unique_ptr<IPacket>> packetsList = network->receiveFromClients();
  while (!packetsList.empty()) {
    unique_ptr<IPacket> packet = move(packetsList.front());
    packetsList.pop_front();

    switch (packet->getType()) {
    case PacketType::MOVEMENT: {
      auto movementPacket = static_cast<MovementPacket *>(packet.get());
      game->updateMovement(movementPacket->objectID,
                           movementPacket->movementType,
                           movementPacket->cameraFront);
      break;
    }
    case PacketType::INTERACTION: {
      auto interactionPacket = static_cast<InteractionPacket *>(packet.get());
      game->updateInteraction(interactionPacket->objectID);
      break;
    }
    case PacketType::CHARACTERSELECT: {
      auto characterPacket = static_cast<CharacterSelectPacket *>(packet.get());
      characterManager->assignCharacter(characterPacket->character,
                                        characterPacket->clientID);
      CharacterResponsePacket responsePacket(
          characterManager->chicken, characterManager->sheep,
          characterManager->pig, characterManager->cow);
      network->sendToAll(responsePacket);
      //if (characterManager->allAssigned()) {
        GameStatePacket statePacket(Gamestate::GAME);
        network->sendToAll(statePacket);
      //}
      break;
    }
    }
  }
  game->applyPhysics();
}

void GameServer::dispatchUpdates() {
  vector<int> updatedObjects = game->getLastUpdatedObjects();
  for (int i = 0; i < updatedObjects.size(); i++) {
    GameObject *obj = game->getObject(updatedObjects[i]);
    ObjectPacket objPacket = ObjectPacket(
        obj->getId(),
        Transform(obj->getPosition(), obj->getRotation(), obj->getScale()),
        obj->isActive());
    network->sendToAll(objPacket);
  }
}