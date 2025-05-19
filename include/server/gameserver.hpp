#pragma once

#include "server_gamestate.hpp"
#include "server_network.hpp"

using namespace std;

class GameServer {
private:
  // Network and GameState
  unique_ptr<ServerNetwork> network;
  unique_ptr<ServerGameState> game;

public:
  GameServer(asio::io_context &io_context, const string &ip,
             const string &port);
  ~GameServer();

  bool start();

  void updateGameState();
  void dispatchUpdates();
};