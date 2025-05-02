#include "client/clientnetwork.hpp"
#include "shared/objects/cube.hpp"
#include "shared/utilities/util_packets.hpp"

/*
 * Constructor for ClientNetwork
 * resolver is used to find all endpoints of an ip and port combination
 * (essentially find devices on that ip:port) connect tries to connect this
 * client's socket to one endpoint found from resolver
 */
ClientNetwork::ClientNetwork(asio::io_context &io_context,
                             const string &ip, const string &port)
    : _socket(io_context) {
  try {
    asio::ip::tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(ip, port);
    asio::connect(_socket, endpoints);
    err = false;
    cout << "Connected to " << ip << ":" << port << endl;
  } catch (exception &e) {
    err = true;
    cerr << "Connection Failed: " << e.what() << endl;
  }
}

/*
 * clean up open socket
 */
ClientNetwork::~ClientNetwork() {
  if (_socket.is_open()) {
    cout << "Sending disconnect packet" << endl;
    send(DisconnectPacket(this->id));
    _socket.shutdown(asio::ip::tcp::socket::shutdown_both);
    _socket.close();
  }
}

void ClientNetwork::send(const IPacket &packet) {
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

deque<unique_ptr<IPacket>> ClientNetwork::receive() {
  deque<unique_ptr<IPacket>> packets;
  while (_socket.available() > 0) {
    PacketType type;
    uint16_t size = 0;
    size_t available;

    if (_socket.read_some(asio::buffer(&type, 1)) < 1) {
      cerr << "Client Warning: Could not read packet type" << endl;
      return packets;
    }

    if (_socket.read_some(asio::buffer(&size, 2)) < 2) {
      cerr << "Client Warning: Could not read packet size" << endl;
      return packets;
    }

    vector<char> payload(size);
    _socket.read_some(asio::buffer(payload));

    packets.push_back(processPackets(static_cast<PacketType>(type), payload));
  }
  return packets;
}

unique_ptr<IPacket> ClientNetwork::processPackets(PacketType type,
                                                       vector<char> payload) {
  switch (type) {
  case PacketType::INIT: {
    unique_ptr<IPacket> packet = deserialize(PacketType::INIT, payload);
    return packet;
  }
  case PacketType::STRING: {
    unique_ptr<IPacket> packet = deserialize(PacketType::STRING, payload);
    return packet;
  }
  case PacketType::POSITION: {
    unique_ptr<IPacket> packet =
        deserialize(PacketType::POSITION, payload);

    // TEST
    if (auto *posPacket = dynamic_cast<PositionPacket *>(packet.get())) {
      printPositionPacket(*posPacket);
    }
    return packet;
  }
  case PacketType::OBJECT: {
    unique_ptr<IPacket> packet = deserialize(PacketType::OBJECT, payload);

    if (auto *objectPacket = dynamic_cast<ObjectPacket *>(packet.get())) {
      // printObjectPacket(*objectPacket);
    }
    return packet;
  }

  default:
    cerr << ("Client Warning: Unknown packet type") << endl;
    return nullptr;
  }
}