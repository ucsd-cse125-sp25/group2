#include "server/servernetwork.hpp"

/*
 * Asio's way of declaring a listener on an ip + port
 * Called and acceptor which takes an io_context and endpoints
 * io_context is basically a core class that allows networking I/O for Asio
*/
ServerNetwork::ServerNetwork(asio::io_context& io_context, const std::string& ip, const std::string& port)
: _acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::make_address(ip), std::stoi(port))),
  client_id(0)
{
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
            this->clients[client_id] = socket;
            InitPacket init(client_id);
            send_to_client(client_id, init);

            // initialize game state and send to client
            game = new GameState();
            send_to_client(client_id, game->init());

            this->client_id++;
        } else {
            std::cerr << "Accept failed: " << ec.message() << "\n";
        }

        accept_client();
    });
}

/*
 * Core communication to client
 * need to send a packet that inherits IPacket 
*/
void ServerNetwork::send_to_client(unsigned int id, const IPacket& packet) {
    auto socket = clients[id];
    vector<char> body = packet.serialize();
    uint16_t body_size = static_cast<uint16_t>(body.size());
    
    vector<char> buffer;
    buffer.reserve(1 + 2 + body.size());

    // Header format: [PacketType (1 byte)][PayloadSize (2 bytes)][Payload]
    buffer.push_back(static_cast<uint8_t>(packet.get_type()));
    buffer.push_back(static_cast<uint8_t>(body_size & 0xFF));
    buffer.push_back(static_cast<uint8_t>((body_size >> 8) & 0xFF));
    buffer.insert(buffer.end(), body.begin(), body.end());
    
    asio::write(*socket, asio::buffer(buffer));
}
/*
 * Send packet to all clients known to server
 * need to send a packet that inherits IPacket 
*/
void ServerNetwork::send_to_all(const IPacket& packet) {
    for (const auto&[id, socket] : clients) {
        vector<char> body = packet.serialize();
        uint16_t body_size = static_cast<uint16_t>(body.size());
        
        vector<char> buffer;
        buffer.reserve(1 + 2 + body.size());

        // Header format: [PacketType (1 byte)][PayloadSize (2 bytes)][Payload]
        buffer.push_back(static_cast<uint8_t>(packet.get_type()));
        buffer.push_back(static_cast<uint8_t>(body_size & 0xFF));
        buffer.push_back(static_cast<uint8_t>((body_size >> 8) & 0xFF));
        buffer.insert(buffer.end(), body.begin(), body.end());
        
        asio::write(*socket, asio::buffer(buffer));
    }
}

void ServerNetwork::receive_from_clients() {
    for (const auto&[id, socket] : clients) {
        PacketType type;
        uint16_t size = 0;
        size_t available;
        if ((available = socket->available()) > 0) {
    
            if (socket->read_some(asio::buffer(&type, 1)) <= 0) {
                std::cerr << "did not read packet type" << std::endl;
                continue;
            }
    
            if (socket->read_some(asio::buffer(&size, 2)) <= 0) {
                std::cerr << "did not read packet type" << std::endl;
                continue;
            }
    
            std::vector<char> payload(size);
            socket->read_some(asio::buffer(payload));
    
            process_packets(static_cast<PacketType>(type),payload,size);
        }
    }
}

void ServerNetwork::process_packets(PacketType type, vector<char> payload, uint16_t size) {
    switch (type) {
        case PacketType::INIT:
            {
                std::unique_ptr<IPacket> packet = deserialize(PacketType::INIT, payload, size);
                break;
            }
        case PacketType::STRING:
            {
                std::unique_ptr<IPacket> packet = deserialize(PacketType::STRING, payload, size);
                break;
            }
        default:
            throw std::runtime_error("Unknown packet type server");
    }
}