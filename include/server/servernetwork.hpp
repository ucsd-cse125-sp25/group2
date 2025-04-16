#pragma once

#include <asio.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <map>
#include "shared/packets.hpp"


#define CLIENT_ID unsigned int

using asio::ip::tcp;

class ServerNetwork {
    private:
        unsigned int client_id;
        asio::ip::tcp::acceptor _acceptor;
        std::map<CLIENT_ID, std::shared_ptr<asio::ip::tcp::socket>> clients;
        void accept_client();
        void process_packets(PacketType type, vector<char> payload, uint16_t size);
    public:
        ServerNetwork(asio::io_context& io_context, const std::string& ip, const std::string& port);
        void start();
        void send_to_client(unsigned int id, const Ipacket& packet);
        void send_to_all(const Ipacket& packet);
        void receive_from_clients();
};