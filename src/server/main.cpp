#include "server/servernetwork.hpp"

int main() {
    try {
        asio::io_context io_context;
        ServerNetwork server(io_context, "127.0.0.1", "12345");
        server.start();
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Server exception: " << e.what() << "\n";
    }

    return 0;
}