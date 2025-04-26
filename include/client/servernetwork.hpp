#pragma once

#include <asio.hpp>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

using asio::ip::tcp;

class ServerNetwork {
private:
  asio::ip::tcp::acceptor _acceptor;
  void accept_client();
  void handle_client(std::shared_ptr<asio::ip::tcp::socket> socket);

public:
  ServerNetwork(asio::io_context &io_context, const std::string &ip,
                const std::string &port);
  void start();
};