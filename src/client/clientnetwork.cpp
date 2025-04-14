#include "client/clientnetwork.hpp"

/*
 * Constructor for ClientNetwork
 * resolver is used to find all endpoints of an ip and port combination (essentially find devices on that ip:port)
 * connect tries to connect this client's socket to one endpoint found from resolver
*/
ClientNetwork::ClientNetwork(asio::io_context& io_context, const std::string& ip, const std::string& port): _socket(io_context) {
    try {
        asio::ip::tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(ip, port);
        asio::connect(_socket, endpoints);
        std::cout << "Connected to " << ip << ":" << port << "\n";
    } catch (std::exception& e) {
        std::cerr << "Connection failed: " << e.what() << "\n";
    }
} 

/*
 * clean up open socket
*/
ClientNetwork::~ClientNetwork() {
    if (_socket.is_open()) {
        _socket.close();
    }
}

/*
 * Asio's way of writing a message over the network
*/
void ClientNetwork::send(const std::string& message) {
    asio::write(_socket, asio::buffer(message));
}

/*
 * Needs to be change but basic implementation of receive messages from server
 * reads socket and attempts to fill reply buffer
 * returns server message back
*/
std::string ClientNetwork::receive() {
    char reply[1024];
    size_t length = _socket.read_some(asio::buffer(reply));
    return std::string(reply, length);
}