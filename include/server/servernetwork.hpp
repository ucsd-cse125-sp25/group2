#pragma once

#include "shared/packets.hpp"

#include <asio.hpp>
#include <deque>
#include <iostream>
#include <map>
#include <memory>
#include <thread>

#define CLIENT_ID unsigned int

using asio::ip::tcp;

class ServerNetwork {
private:
  unsigned int client_id;
  asio::ip::tcp::acceptor _acceptor;
  std::map<CLIENT_ID, std::shared_ptr<asio::ip::tcp::socket>> clients;

  void acceptClient();
  std::unique_ptr<IPacket> processPackets(PacketType type,
                                          vector<char> payload);

public:
  ServerNetwork(asio::io_context &io_context, const std::string &ip,
                const std::string &port);
  void start();
  void sendToClient(unsigned int id, const IPacket &packet);
  void sendToAll(const IPacket &packet);
  void handleClientDisconnect(CLIENT_ID id);
  deque<std::unique_ptr<IPacket>> receiveFromClients();
};