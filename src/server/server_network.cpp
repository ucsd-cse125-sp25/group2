#include "server_network.hpp"

/*
 * Asio's way of declaring a listener on an ip + port
 * Called and acceptor which takes an io_context and endpoints
 * io_context is basically a core class that allows networking I/O for Asio
 */
ServerNetwork::ServerNetwork(asio::io_context &io_context, const string &ip,
                             const string &port)
    : _acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::make_address(ip),
                                                    stoi(port))),
      clients(), clientID(0) {
  cout << "Server started on " << ip << ":" << port << endl;
}

void ServerNetwork::start() { acceptClient(); }

/*
 * Asynchronously accept clients
 * Essentially tells io_context hey whenever client comes do the lambda function
 * All lambda function says is that socket that was created and use it to handle
 * client messages Accept_client again to keep listening for new clients
 */
void ServerNetwork::acceptClient() {
  auto socket = make_shared<asio::ip::tcp::socket>(_acceptor.get_executor());

  _acceptor.async_accept(*socket, [this, socket](error_code ec) {
    if (!ec) {
      cout << "New client connected" << endl;
      this->clients[clientID] = socket;
      InitPacket init(clientID);
      sendToClient(clientID, init);
      /*
      // initialize game state and send to client
      sendToClient(clientID, game->init());
      */
      clientID++;
    } else {
      cerr << "Accept Failed: " << ec.message() << endl;
    }

    acceptClient();
  });
}

/*
 * Core communication to client
 * need to send a packet that inherits IPacket
 */
void ServerNetwork::sendToClient(unsigned int id, const IPacket &packet) {
  auto socket = clients[id];
  if (!socket->is_open()) {
    handleClientDisconnect(id);
    return;
  }
  vector<char> body = packet.serialize();
  uint16_t body_size = static_cast<uint16_t>(body.size());

  vector<char> buffer;
  buffer.reserve(1 + 2 + body.size());

  // Header format: [PacketType (1 byte)][PayloadSize (2 bytes)][Payload]
  buffer.push_back(static_cast<uint8_t>(packet.getType()));
  buffer.push_back(static_cast<uint8_t>(body_size & 0xFF));
  buffer.push_back(static_cast<uint8_t>((body_size >> 8) & 0xFF));
  buffer.insert(buffer.end(), body.begin(), body.end());

  try {
    asio::write(*socket, asio::buffer(buffer));
  } catch (const system_error &e) {
    cerr << "Write failed: " << e.what() << endl;
    handleClientDisconnect(id);
  }
}

/*
 * Send packet to all clients known to server
 * need to send a packet that inherits IPacket
 */
void ServerNetwork::sendToAll(const IPacket &packet) {
  for (const auto &[id, socket] : clients) {
    sendToClient(id, packet);
  }
}

deque<unique_ptr<IPacket>> ServerNetwork::receiveFromClients() {
  deque<unique_ptr<IPacket>> packets;
  for (auto it = clients.begin(); it != clients.end();) {
    auto &socket = it->second;
    asio::error_code ec;

    while (socket->available() > 0) {
      PacketType type;
      uint16_t size = 0;

      if (socket->read_some(asio::buffer(&type, 1), ec) <= 0 || ec)
        break;

      if (socket->read_some(asio::buffer(&size, 2), ec) <= 0 || ec)
        break;

      vector<char> payload(size);
      if (socket->read_some(asio::buffer(payload, size), ec) <= 0 || ec)
        break;

      packets.push_back(processPackets(static_cast<PacketType>(type), payload));
    }
    if (ec == asio::error::eof || ec == asio::error::connection_reset) {
      handleClientDisconnect(it->first);
    }
    ++it;
  }
  return packets;
}

unique_ptr<IPacket> ServerNetwork::processPackets(PacketType type,
                                                  vector<char> payload) {
  switch (type) {
  case PacketType::INIT: {
    unique_ptr<IPacket> packet = deserialize(PacketType::INIT, payload);
    return packet;
  }
  case PacketType::MOVEMENT: {
    unique_ptr<IPacket> packet = deserialize(PacketType::MOVEMENT, payload);
    return packet;
  }
  case PacketType::INTERACTION: {
    unique_ptr<IPacket> packet = deserialize(PacketType::INTERACTION, payload);
    return packet;
  }
  case PacketType::DISCONNECT: {
    unique_ptr<IPacket> packet = deserialize(PacketType::DISCONNECT, payload);
    return packet;
  }
  default:
    cerr << ("Server Warning: Unknown packet type") << endl;
    return nullptr;
  }
}

void ServerNetwork::handleClientDisconnect(CLIENT_ID id) {
  cerr << "Client " << id << " Disconnected" << endl;
  auto socket = clients[id];
  if (socket->is_open()) {
    socket->close();
  }
  clients.erase(id);
}