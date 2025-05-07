#include "server/gameserver.hpp"
#include <chrono>
#include <iostream>

using namespace chrono_literals;
using namespace chrono;

#define SERVERTICKS milliseconds(30)

int main() {
  try {
    // Create context and start the server
    asio::io_context io_context;
    GameServer server(io_context, "127.0.0.1", "12345");
    server.start();
    int count = 0;

    // Server loop
    while (true) {
      auto start = high_resolution_clock::now();

      server.updateGameState();
      server.dispatchUpdates();

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

  return 0;
}