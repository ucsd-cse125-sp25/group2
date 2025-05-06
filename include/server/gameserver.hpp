#pragma once

#include "server/servergamestate.hpp"
#include "server/servernetwork.hpp"

#include <memory>
#include <string>

using namespace std;

class GameServer {
private:
  unique_ptr<ServerGameState> game;
  void handleAction(unique_ptr<ActionPacket>);

public:
  unique_ptr<ServerNetwork> network;
  GameServer(asio::io_context &io_context, const string &ip,
             const string &port);
  ~GameServer();
  void start();
  void updateGameState();
  void updateClients();
  void handleAction(ActionPacket *packet);
};