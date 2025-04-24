#pragma once

#include "server/servernetwork.hpp"

class Server {
    private:
        GameState* game;
    public:
        ServerNetwork* network;
        Server(asio::io_context& io_context, const std::string& ip, const std::string& port);
        ~Server();
        void start();
        void update();
};