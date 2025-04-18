#include "server/servernetwork.hpp"
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;
using namespace std::chrono;

#define SERVERTICKS milliseconds(30)

int main() {
    try {
        // Create context and start the server
        asio::io_context io_context;
        ServerNetwork server(io_context, "127.0.0.1", "12345");
        server.start();
        int count = 0;

        // Server loop
        while (true) {
            auto start = high_resolution_clock::now();

            server.receive_from_clients();
            
            auto stop = high_resolution_clock::now();
            auto wait = duration_cast<milliseconds>(SERVERTICKS - (stop - start));

            if (wait <= 0ms) {
                std::cerr << "Warning: Exceeded Server Tick";
            } else {
                io_context.run_for(wait);
            }
        }
    } catch (std::exception& e) {
        std::cerr << "Server Exception: " << e.what() << std::endl;
    }

    return 0;
}