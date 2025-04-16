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

        //printf("type %d size: %d packet size %ld\n", (uint8_t)type, size, sizeof(init_packet));
        switch (static_cast<PacketType>(type)) {
            case PacketType::INIT:
                {
                    std::cout << "entered init" << std::endl;
                    init_packet init;
                    memcpy(&init.client_id,payload.data(),size);
                    printf("init %d\n", init.client_id);
                    return true;
                }
            case PacketType::STRING:
                {
                    string_packet string;
                    memcpy(&string.message,payload.data(),size);
                    printf("Server Message: %s\n", string.message.c_str());
                    return true;
                }
        }
    }
    return false;
}