#pragma once

#include "shared/packets.hpp"
#include "shared/gamestate.hpp"

#include <asio.hpp>
#include <iostream>
#include <thread>
#include <deque>
#include <memory>
#include <map>

#define CLIENT_ID unsigned int

using asio::ip::tcp;

class ServerNetwork {
    private:
        unsigned int client_id;
        asio::ip::tcp::acceptor _acceptor;
        std::map<CLIENT_ID, std::shared_ptr<asio::ip::tcp::socket>> clients;
        GameState* game;

        void accept_client();
        std::unique_ptr<IPacket> process_packets(PacketType type, vector<char> payload);
    public:
        ServerNetwork(asio::io_context& io_context, const std::string& ip, const std::string& port, GameState* game);
        void start();
        void send_to_client(unsigned int id, const IPacket& packet);
        void send_to_all(const IPacket& packet);
        void handle_client_disconnect(CLIENT_ID id);
        deque<std::unique_ptr<IPacket>> receive_from_clients();
};