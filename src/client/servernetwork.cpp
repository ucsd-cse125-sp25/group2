#include "client/servernetwork.hpp"

/*
 * Asio's way of declaring a listener on an ip + port
 * Called and acceptor which takes an io_context and endpoints
 * io_context is basically a core class that allows networking I/O for Asio
*/
ServerNetwork::ServerNetwork(asio::io_context& io_context, const std::string& ip, const std::string& port)
: _acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::make_address(ip), std::stoi(port))) {
    std::cout << "Server started on " << ip << ":" << port << "\n";
}


void ServerNetwork::start() {
    accept_client();
}

/*
 * Asynchronously accept clients
 * Essentially tells io_context hey whenever client comes do the lambda function
 * All lambda function says is that socket that was created and use it to handle client messages
 * Accept_client again to keep listening for new clients
*/
void ServerNetwork::accept_client() {
    auto socket = std::make_shared<asio::ip::tcp::socket>(_acceptor.get_executor());

    _acceptor.async_accept(*socket, [this, socket](std::error_code ec) {
        if (!ec) {
            std::cout << "New client connected\n";
            handle_client(socket);
        } else {
            std::cerr << "Accept failed: " << ec.message() << "\n";
        }

        accept_client();
    });
}

/*
 * Core communication between a client and server
 * for this client-server socket keep reading the socket to see if any client messages
 * then respond
*/
void ServerNetwork::handle_client(std::shared_ptr<asio::ip::tcp::socket> socket) {
    std::thread([socket]() {
        try {
            char data[1024];
            while (true) {
                size_t length = socket->read_some(asio::buffer(data));
                std::cout << "Received message: " << std::string(data, length) << "\n";

                std::string response = "Server: Message received!";
                asio::write(*socket, asio::buffer(response));
            }
        } catch (std::exception& e) {
            std::cerr << "Client communication failed: " << e.what() << "\n";
        }
    }).detach();
}