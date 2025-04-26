#include "client/clientnetwork.hpp"
#include "shared/cube.hpp"
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
        std::cout << "Connected to " << ip << ":" << port << std::endl;
    } catch (std::exception& e) {
        err = true;
        std::cerr << "Connection Failed: " << e.what() << std::endl;
    }
} 

void ClientNetwork::set_id(CLIENT_ID id) {
    this->id = id;
}

/*
 * clean up open socket
*/
ClientNetwork::~ClientNetwork() {
    if (_socket.is_open()) {
        std::cout << "Sending disconnect packet" << std::endl;
        send(DisconnectPacket(this->id));
        _socket.shutdown(asio::ip::tcp::socket::shutdown_both);
        _socket.close();
    }
}

void ClientNetwork::send(const IPacket& packet) {
    vector<char> body = packet.serialize();
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

deque<std::unique_ptr<IPacket>> ClientNetwork::receive() {
    deque<std::unique_ptr<IPacket>> packets;
    while (_socket.available() > 0) {
        PacketType type;
        uint16_t size = 0;
        size_t available;

        if (_socket.read_some(asio::buffer(&type, 1)) < 1) {
            std::cerr << "Client Warning: Could not read packet type" << std::endl;
            return packets;
        }

        if (_socket.read_some(asio::buffer(&size, 2)) < 2) {
            std::cerr << "Client Warning: Could not read packet size" << std::endl;
            return packets;
        }

        std::vector<char> payload(size);
        _socket.read_some(asio::buffer(payload));

        packets.push_back(process_packets(static_cast<PacketType>(type),payload,size));
    }
    return packets;
}

std::unique_ptr<IPacket> ClientNetwork::process_packets(PacketType type, vector<char> payload, uint16_t size) {
    switch (type) {
        case PacketType::INIT:
            {
                std::unique_ptr<IPacket> packet = deserialize(PacketType::INIT, payload, size);
                return packet;
            }
        case PacketType::STRING:
            {
                std::unique_ptr<IPacket> packet = deserialize(PacketType::STRING, payload, size);
                return packet;
            }
        case PacketType::POSITION:
            {
                std::unique_ptr<IPacket> packet = deserialize(PacketType::POSITION, payload, size);

                // TEST
                if (auto* posPacket = dynamic_cast<PositionPacket*>(packet.get())) {
                    std::cout << "x: " << posPacket->x << std::endl;
                    std::cout << "y: " << posPacket->y << std::endl;
                    std::cout << "z: " << posPacket->z << std::endl;
                }
                return packet;
            }
        
        default:
            std::cerr << ("Client Warning: Unknown packet type") << std::endl;
            return nullptr;
    }
}