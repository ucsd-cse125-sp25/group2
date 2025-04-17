#include "server/servernetwork.hpp"
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;
using namespace std::chrono;

#define SERVERTICKS milliseconds(30)

int main() {
    try {
        asio::io_context io_context;
        ServerNetwork server(io_context, "127.0.0.1", "12345");
        server.start();
        int count = 0;

        while (true) {
            auto start = high_resolution_clock::now();

            server.receive_from_clients();
            
            auto stop = high_resolution_clock::now();
            auto wait = duration_cast<milliseconds>(SERVERTICKS - (stop - start));
            //std::cout << wait.count() << " wait time: " << count++ << std::endl;

            if (wait <= 0ms) {
                std::cerr << "Warning went over server tick";
            } else {
                io_context.run_for(wait);
            }
        }
    } catch (std::exception& e) {
        std::cerr << "Server exception: " << e.what() << "\n";
    }

    return 0;
}