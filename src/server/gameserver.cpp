#include "server/gameserver.hpp"

GameServer::GameServer(asio::io_context &io_context, const string &ip,
                       const string &port) {
  game = make_unique<ServerGameState>();
  network = make_unique<ServerNetwork>(io_context, ip, port);
}

GameServer::~GameServer() {}

void GameServer::start() {
  network->start();
  if (!game->init()) {
    cerr << "ServerGameState initialization failed" << endl;
    return;
  }
}

void GameServer::updateGameState() {
  deque<unique_ptr<IPacket>> list_packets = network->receiveFromClients();
  while (!list_packets.empty()) {
    // i dont like this change it to smartvector later
    unique_ptr<IPacket> packet = move(list_packets.front());
    list_packets.pop_front();

    switch (packet->getType()) {
    case PacketType::MOVEMENT:
      auto movementPacket = static_cast<MovementPacket *>(packet.get());
      game->updateMovement(movementPacket->objectID,
                           movementPacket->movementType, movementPacket->cameraFront);
      break;
    }
  }
}

void GameServer::dispatchUpdates() {
  vector<int> updatedObjects = game->getLastUpdatedObjects();
  for (int i = 0; i < updatedObjects.size(); i++) {
    GameObject *obj = game->getObject(updatedObjects[i]);
    ObjectPacket objPacket = ObjectPacket(
        obj->getId(), obj->getType(),
        Transform(obj->getPosition(), obj->getRotation(), obj->getScale()),
        obj->isInteractable(), obj->isActive());
    network->sendToAll(objPacket);
  }
}