#include "server/gameserver.hpp"

Server::Server(asio::io_context& io_context, const std::string& ip, const std::string& port) : game(new GameState()) {
    network = new ServerNetwork(io_context,ip,port, game);
}

void Server::start() {
    network->start();
}

void Server::update() {
    deque<std::unique_ptr<IPacket>> list_packets = network->receive_from_clients();

    while (!list_packets.empty()) {
        std::unique_ptr<IPacket> packet = std::move(list_packets.front());
        list_packets.pop_front();

        switch (packet->get_type()) {
            case PacketType::INIT: {
                auto init_packet = static_cast<InitPacket*>(packet.get());
                // Handle initialization packet
                break;
            }
            case PacketType::POSITION: {
                auto position_packet = static_cast<PositionPacket*>(packet.get());
                // Handle position packet
                break;
            }
            case PacketType::STRING: {
                auto string_packet = static_cast<StringPacket*>(packet.get());
                // Handle string packet
                break;
            }
            case PacketType::ACTION: {
                auto action_packet = static_cast<ActionPacket*>(packet.get());
                // Handle action packet
                send_to_all(&game->handleAction(action_packet));
                break;
            }
        }
    }
}

Server::~Server() {
    delete network; 
}