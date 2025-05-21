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

bool ServerNetwork::start() { return acceptClient(); }

/*
 * Asynchronously accept clients
 * Essentially tells io_context hey whenever client comes do the lambda function
 * All lambda function says is that socket that was created and use it to handle
 * client messages Accept_client again to keep listening for new clients
 */
bool ServerNetwork::acceptClient() {
  auto socket = make_shared<asio::ip::tcp::socket>(_acceptor.get_executor());

  _acceptor.async_accept(*socket, [this, socket](error_code ec) {
    if (!ec) {
      cout << "New client connected" << endl;
      clients[clientID] = socket;
      lastMovement[clientID] = MovementType::NONE;
      lastRotation[clientID] = nullptr;
      InitPacket init(clientID);
      sendToClient(clientID, init);
      if (onClientJoin)
        onClientJoin();
      clientID++;
    } else {
      cerr << "Accept Failed: " << ec.message() << endl;
    }
    acceptClient();
  });
  return true;
}

/*
 * Core communication to client
 * need to send a packet that inherits IPacket
 */
void ServerNetwork::sendToClient(CLIENT_ID id, const IPacket &packet) {
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
  for (auto it = clients.begin(); it != clients.end();) {
    auto current = it++;
    sendToClient(current->first, packet);
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

      unique_ptr<IPacket> packet =
          processPackets(static_cast<PacketType>(type), payload, it->first);
      if (packet) {
        packets.push_back(move(packet));
      }
    }
    if (ec == asio::error::eof || ec == asio::error::connection_reset) {
      handleClientDisconnect(it->first);
    }
    lastMovement[it->first] = MovementType::NONE;
    it++;
  }

  // add latest rotation packets for each client
  for (auto &[clientId, packet] : lastRotation) {
    if (packet) {
      packets.push_back(move(packet));
    }
  }
  lastRotation.clear();

  return packets;
}

unique_ptr<IPacket> ServerNetwork::processPackets(PacketType type,
                                                  vector<char> payload,
                                                  CLIENT_ID id) {
  switch (type) {
  case PacketType::INIT: {
    unique_ptr<IPacket> packet = deserialize(PacketType::INIT, payload);
    return packet;
  }
  case PacketType::MOVEMENT: {
    unique_ptr<IPacket> packet = deserialize(PacketType::MOVEMENT, payload);
    auto movementPacket = static_cast<MovementPacket *>(packet.get());
    if (lastMovement[clientID] != movementPacket->movementType) {
      lastMovement[clientID] = movementPacket->movementType;
      return packet;
    }
    return nullptr;
  }
  case PacketType::ROTATION: {
    unique_ptr<IPacket> packet = deserialize(PacketType::ROTATION, payload);
    lastRotation[clientID] = move(packet);
    return nullptr;
  }
  case PacketType::INTERACTION: {
    unique_ptr<IPacket> packet = deserialize(PacketType::INTERACTION, payload);
    return packet;
  }
  case PacketType::CHARACTERSELECT: {
    unique_ptr<IPacket> packet =
        deserialize(PacketType::CHARACTERSELECT, payload);
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
  onClientLeave(id);
}

void ServerNetwork::setOnJoin(function<void()> callback) {
  onClientJoin = callback;
}
void ServerNetwork::setOnLeave(function<void(int)> callback) {
  onClientLeave = callback;
}