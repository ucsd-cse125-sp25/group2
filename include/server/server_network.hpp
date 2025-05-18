#pragma once

#include "packets.hpp"
#include <asio.hpp>
#include <deque>
#include <iostream>
#include <map>
#include <memory>
#include <thread>
#include <functional>

using asio::ip::tcp;
using namespace std;

class ServerNetwork {
private:
  CLIENT_ID clientID;
  asio::ip::tcp::acceptor _acceptor;
  map<CLIENT_ID, shared_ptr<asio::ip::tcp::socket>> clients;

  void acceptClient();
  unique_ptr<IPacket> processPackets(PacketType type, vector<char> payload);

public:
  ServerNetwork(asio::io_context &io_context, const string &ip,
                const string &port);
  void start();
  void sendToClient(unsigned int id, const IPacket &packet);
  void sendToAll(const IPacket &packet);
  void handleClientDisconnect(CLIENT_ID id);
  deque<unique_ptr<IPacket>> receiveFromClients();

  void setOnJoin(function<void()> callback);
  void setOnLeave(function<void(int)> callback);

  function<void()> onClientJoin;
  function<void(int)> onClientLeave;
};