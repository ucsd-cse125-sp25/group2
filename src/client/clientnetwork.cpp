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
        err = false;
        std::cout << "Connected to " << ip << ":" << port << "\n";
    } catch (std::exception& e) {
        err = true;
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

void ClientNetwork::send(const Ipacket& packet) {
    vector<char> body = packet.serialize();
    //printf("sending size: %ld\n", body.size());
    uint16_t body_size = static_cast<uint16_t>(body.size());
    
    vector<char> buffer;
    buffer.reserve(1 + 2 + body.size());

    // Header format: [PacketType (1 byte)][PayloadSize (2 bytes)][Payload]
    buffer.push_back(static_cast<uint8_t>(packet.get_type()));
    buffer.push_back(static_cast<char>(body_size & 0xFF));    
    buffer.push_back(static_cast<char>((body_size >> 8) & 0xFF));     
    buffer.insert(buffer.end(), body.begin(), body.end());
    
    asio::write(_socket, asio::buffer(buffer));
}

bool ClientNetwork::receive() {
    PacketType type;
    uint16_t size = 0;
    size_t available;
    if ((available = _socket.available()) > 0) {

        if (_socket.read_some(asio::buffer(&type, 1)) < 1) {
            std::cerr << "did not read packet type" << std::endl;
            return false;
        }

        if (_socket.read_some(asio::buffer(&size, 2)) < 2) {
            std::cerr << "did not read packet size" << std::endl;
            return false;
        }

        std::vector<char> payload(size);
        _socket.read_some(asio::buffer(payload));

        return process_packets(static_cast<PacketType>(type),payload,size);
    }
    return false;
}

bool ClientNetwork::process_packets(PacketType type, vector<char> payload, uint16_t size) {
    switch (type) {
        case PacketType::INIT:
            {
                std::unique_ptr<Ipacket> packet = deserialize(PacketType::INIT, payload, size);
                return true;
            }
        case PacketType::STRING:
            {
                std::unique_ptr<Ipacket> packet = deserialize(PacketType::STRING, payload, size);
                return true;
            }
        case PacketType::POSITION:
            {
                std::unique_ptr<Ipacket> packet = deserialize(PacketType::POSITION, payload, size);

                // TEST
                if (auto* posPacket = dynamic_cast<PositionPacket*>(packet.get())) {
                    std::cout << "x: " << posPacket->x << std::endl;
                    std::cout << "y: " << posPacket->y << std::endl;
                    std::cout << "z: " << posPacket->z << std::endl;
                }
                return true;
            }
        default:
            throw std::runtime_error("Unknown packet type");
            return false;
    }
}