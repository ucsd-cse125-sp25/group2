#pragma once 
#include "shared/packets.hpp"
#include <iostream>
#include <asio.hpp>
#include <deque>

using asio::ip::tcp;

class ClientNetwork
{
private:
    tcp::socket _socket;

    std::unique_ptr<IPacket> process_packets(PacketType type, vector<char> payload, uint16_t size);
public:
    bool err;

    // ctor/dtor
    ClientNetwork(asio::io_context& io_context, const std::string& ip, const std::string& port);
    ~ClientNetwork(void);

    void send(const IPacket& packet);

    deque<std::unique_ptr<IPacket>> receive();
};