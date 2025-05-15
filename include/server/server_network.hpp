#pragma once

#include "globals.hpp"
#include "packets.hpp"

#include <asio.hpp>
#include <deque>
#include <iostream>
#include <map>
#include <memory>
#include <thread>
#include <unordered_map>

using asio::ip::tcp;
using namespace std;

#define CLIENT_ID unsigned int

class ServerNetwork {
private:
  CLIENT_ID clientID;
  asio::ip::tcp::acceptor _acceptor;
  map<CLIENT_ID, shared_ptr<asio::ip::tcp::socket>> clients;

  void acceptClient();
  unique_ptr<IPacket> processPackets(PacketType type, vector<char> payload,
                                     int clientID);

  unordered_map<int, MovementType> lastMovement;

public:
  ServerNetwork(asio::io_context &io_context, const string &ip,
                const string &port);
  void start();
  void sendToClient(unsigned int id, const IPacket &packet);
  void sendToAll(const IPacket &packet);
  void handleClientDisconnect(CLIENT_ID id);
  deque<unique_ptr<IPacket>> receiveFromClients();
};