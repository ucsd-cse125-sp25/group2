#pragma once 
#include <iostream>
#include <asio.hpp>

using asio::ip::tcp;

class ClientNetwork
{
private:
    tcp::socket _socket;
public:

    // ctor/dtor
    ClientNetwork(asio::io_context& io_context, const std::string& ip, const std::string& port);
    ~ClientNetwork(void);

    void send(const std::string& message);

    std::string receive();
};