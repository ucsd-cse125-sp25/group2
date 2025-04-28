#pragma once

#include "server/servernetwork.hpp"
#include "server/servergamestate.hpp"

#include <memory>

class GameServer {
private:
  std::unique_ptr<GameState> game;

public:
  std::unique_ptr<ServerNetwork> network;
  GameServer(asio::io_context &io_context, const std::string &ip,
             const std::string &port);
  ~GameServer();
  void start();
  void update();
};