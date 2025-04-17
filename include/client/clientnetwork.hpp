#pragma once 
#include <iostream>
#include <asio.hpp>
#include "shared/packets.hpp"

using asio::ip::tcp;

class ClientNetwork
{
private:
    tcp::socket _socket;

    bool process_packets(PacketType type, vector<char> payload, uint16_t size);
public:
    bool err;

    // ctor/dtor
    ClientNetwork(asio::io_context& io_context, const std::string& ip, const std::string& port);
    ~ClientNetwork(void);

    void send(const IPacket& packet);

    bool receive();
};