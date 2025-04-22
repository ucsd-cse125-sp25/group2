#include "server\gameserver.hpp"

Server::Server(asio::io_context& io_context, const std::string& ip, const std::string& port) {
    network = new ServerNetwork(io_context,ip,port);
}

void Server::start() {
    network->start();
}

void Server::update() {
    deque<std::unique_ptr<IPacket>> list_packets = network->receive_from_clients();
}

Server::~Server() {
    delete network; 
}