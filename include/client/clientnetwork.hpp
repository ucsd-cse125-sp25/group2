#pragma once 
#include "shared/packets.hpp"
#include <iostream>
#include <asio.hpp>
#include <deque>

using asio::ip::tcp;

#define CLIENT_ID unsigned int

class ClientNetwork
{
private:
    tcp::socket _socket;
    CLIENT_ID id;

    std::unique_ptr<IPacket> process_packets(PacketType type, vector<char> payload);
public:
    bool err;

    // ctor/dtor
    ClientNetwork(asio::io_context& io_context, const std::string& ip, const std::string& port);
    ~ClientNetwork(void);

    void send(const IPacket& packet);
    void set_id(CLIENT_ID id);

    deque<std::unique_ptr<IPacket>> receive();
};