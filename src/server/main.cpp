#include "gameserver.hpp"
#include <chrono>
#include <iostream>

using namespace chrono_literals;
using namespace chrono;

#define SERVERTICKS milliseconds(30)

int main() {
  // Create context and start the server
  asio::io_context io_context;
  unique_ptr <GameServer> server(new GameServer(io_context, "127.0.0.1", "12345"));
  if (!server->start()) {
    cerr << "Server Initialization Failed" << endl;
    exit(EXIT_FAILURE);
  }

  try {
    // Server loop
    while (true) {
      auto start = high_resolution_clock::now();

      server->updateGameState();
      server->dispatchUpdates();

      auto stop = high_resolution_clock::now();
      auto wait = duration_cast<milliseconds>(SERVERTICKS - (stop - start));

      if (wait <= 0ms) {
        cerr << "Warning: Exceeded Server Tick";
      } else {
        io_context.run_for(wait);
      }
    }
  } catch (exception &e) {
    cerr << "Server Exception: " << e.what() << endl;
  }

  exit(EXIT_SUCCESS);
}