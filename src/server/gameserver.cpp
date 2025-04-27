#include "server/gameserver.hpp"

GameServer::GameServer(asio::io_context &io_context, const std::string &ip,
               const std::string &port) {
  game = std::make_unique<GameState>();
  network = std::make_unique<ServerNetwork>(io_context, ip, port, game.get());
}

GameServer::~GameServer() {}

void GameServer::start() { network->start(); }

void GameServer::update() {
  deque<std::unique_ptr<IPacket>> list_packets =
      network->receiveFromClients();

  while (!list_packets.empty()) {
    std::unique_ptr<IPacket> packet = std::move(list_packets.front());
    list_packets.pop_front();

    switch (packet->get_type()) {
    case PacketType::INIT: {
      auto init_packet = static_cast<InitPacket *>(packet.get());
      // Handle initialization packet
      break;
    }
    case PacketType::POSITION: {
      auto position_packet = static_cast<PositionPacket *>(packet.get());
      // Handle position packet
      break;
    }
    case PacketType::ACTION: {
      auto action_packet = static_cast<ActionPacket *>(packet.get());
      // Handle action packet
      network->sendToAll(game->handleAction(action_packet));
      break;
    }
    case PacketType::OBJECT: {
      auto object_packet = static_cast<ObjectPacket *>(packet.get());
      break;
    }
    case PacketType::DISCONNECT: {
      auto disconnect_packet = static_cast<DisconnectPacket *>(packet.get());
      network->handleClientDisconnect(disconnect_packet->client_id);
      break;
    }
    }
  }
}
