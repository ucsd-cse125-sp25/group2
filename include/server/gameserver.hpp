#pragma once

#include "server_character_manager.hpp"
#include "server_gamestate.hpp"
#include "server_network.hpp"
#include <nlohmann/json.hpp>

using namespace std;

class GameServer {
private:
  // Network and GameState
  unique_ptr<ServerNetwork> network;
  unique_ptr<ServerGameState> game;
  unique_ptr<CharacterManager> characterManager;

  static json loadConfig(const std::string& path);

public:
  GameServer(asio::io_context &io_context);
  ~GameServer();

  bool start();

  void updateGameState();
  void dispatchUpdates();

  void readConfig();
};