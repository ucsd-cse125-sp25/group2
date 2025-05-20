#pragma once

#include "packets.hpp"

#include <asio.hpp>
#include <deque>
#include <iostream>

using asio::ip::tcp;
using namespace std;

class ClientNetwork {
private:
  CLIENT_ID id;
  tcp::socket _socket;

  unique_ptr<IPacket> processPackets(PacketType type, vector<char> payload);

public:
  bool err;

  // ctor/dtor
  ClientNetwork(asio::io_context &io_context, const string &ip,
                const string &port);
  ~ClientNetwork();

  void send(const IPacket &packet);
  deque<unique_ptr<IPacket>> receive();

  void setId(CLIENT_ID id) { this->id = id; }
  CLIENT_ID getId() { return id; }
};