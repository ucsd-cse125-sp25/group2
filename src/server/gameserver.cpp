#include "server/gameserver.hpp"

GameServer::GameServer(asio::io_context &io_context, const string &ip,
                       const string &port) {
  game = make_unique<ServerGameState>();
  network = make_unique<ServerNetwork>(io_context, ip, port);
}

GameServer::~GameServer() {}

void GameServer::start() { network->start(); }

void GameServer::updateGameState() {
  deque<std::unique_ptr<IPacket>> list_packets = network->receiveFromClients();
  while (!list_packets.empty()) {
    // i dont like this change it to smartvector later
    std::unique_ptr<IPacket> packet = std::move(list_packets.front());
    list_packets.pop_front();

    switch (packet->getType()) {
    case PacketType::MOVEMENT:
      auto movementPacket = static_cast<MovementPacket *>(packet.get());
      game->updateMovement(movementPacket->objectID, movementPacket->movementType);
      break;
    }
  }
}

void GameServer::updateClients() {
  vector<int> object_ids = (*game).getLastUpdatedObjects();
  for (int i = 0; i < object_ids.size(); i++) {
    GameObject *obj = (*game).getObject(object_ids[i]);
    ObjectPacket obj_packet = ObjectPacket(
        obj->getId(), obj->getType(),
        Transform(obj->getPosition(), obj->getRotation(), obj->getScale()),
        obj->isInteractable(), obj->isActive());
    network->sendToAll(obj_packet);
  }
}