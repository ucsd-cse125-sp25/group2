#include "gameserver.hpp"

GameServer::GameServer(asio::io_context &io_context, const string &ip,
                       const string &port) {
  game = make_unique<ServerGameState>();
  network = make_unique<ServerNetwork>(io_context, ip, port);
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
      game->updateMovement(movementPacket->objectID,
                           movementPacket->movementType,
                           movementPacket->cameraFront);
      break;
    }
    case PacketType::ROTATION: {
      auto rotationPacket = static_cast<RotationPacket *>(packet.get());
      game->updateRotation(rotationPacket->objectID, rotationPacket->rotation);
      break;
    }
    case PacketType::INTERACTION: {
      auto interactionPacket = static_cast<InteractionPacket *>(packet.get());
      game->updateInteraction(interactionPacket->objectID);
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