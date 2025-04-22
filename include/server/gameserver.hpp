#pragma once

#include "server/servernetwork.hpp"


class Server {
    public:
        ServerNetwork* network;
        Server(asio::io_context& io_context, const std::string& ip, const std::string& port);
        ~Server();
        void start();
        void update();
};