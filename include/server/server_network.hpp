#pragma once

#include "globals.hpp"
#include "packets.hpp"
#include <asio.hpp>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <thread>
#include <unordered_map>

using asio::ip::tcp;
using namespace std;

class ServerNetwork {
private:
  CLIENT_ID clientID;
  asio::ip::tcp::acceptor _acceptor;
  map<CLIENT_ID, shared_ptr<asio::ip::tcp::socket>> clients;

  unordered_map<int, MovementType> lastMovement;
  unordered_map<int, unique_ptr<IPacket>> lastRotation;

  bool acceptClient();
  unique_ptr<IPacket> processPackets(PacketType type, vector<char> payload,
                                     int clientID);

public:
  ServerNetwork(asio::io_context &io_context, const string &ip,
                const string &port);
  bool start();
  void sendToClient(unsigned int id, const IPacket &packet);
  void sendToAll(const IPacket &packet);
  void handleClientDisconnect(CLIENT_ID id);
  deque<unique_ptr<IPacket>> receiveFromClients();

  void setOnJoin(function<void()> callback);
  void setOnLeave(function<void(int)> callback);

  function<void()> onClientJoin;
  function<void(int)> onClientLeave;
};