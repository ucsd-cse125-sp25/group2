#include "packets.hpp"

vector<char> InitPacket::serialize() const {
  vector<char> buffer(sizeof(CLIENT_ID));
  memcpy(buffer.data(), &id, sizeof(CLIENT_ID));
  return buffer;
}

InitPacket InitPacket::deserialize(const vector<char> &payload) {
  CLIENT_ID id;

  memcpy(&id, payload.data(), sizeof(CLIENT_ID));
  InitPacket packet(id);
  return packet;
}

vector<char> ObjectPacket::serialize() const {
  vector<char> buffer(sizeof(OBJECT_ID) + sizeof(Transform) + sizeof(bool));
  unsigned long size = 0;
  memcpy(buffer.data(), &id, sizeof(OBJECT_ID));
  size += sizeof(OBJECT_ID);
  serializeTransform(buffer.data(), transform, size);
  size += sizeof(bool);
  memcpy(buffer.data() + size, &active, sizeof(bool));
  return buffer;
}

ObjectPacket ObjectPacket::deserialize(const vector<char> &payload) {
  OBJECT_ID id;
  Transform transform;
  bool active;

  unsigned long size = 0;
  memcpy(&id, payload.data(), sizeof(OBJECT_ID));
  size += sizeof(OBJECT_ID);
  deserializeTransform(payload, transform, size);
  memcpy(&active, payload.data() + size, sizeof(bool));
  ObjectPacket packet(id, transform, active);
  packet.active = active;
  return packet;
}

vector<char> GameStatePacket::serialize() const {
  vector<char> buffer(sizeof(uint8_t));
  memcpy(buffer.data(), &state, sizeof(uint8_t));
  return buffer;
}

GameStatePacket GameStatePacket::deserialize(const vector<char> &payload) {
  Gamestate state;

  memcpy(&state, payload.data(), sizeof(uint8_t));
  GameStatePacket packet(state);
  return packet;
}

vector<char> CharacterResponsePacket::serialize() const {
  vector<char> buffer(sizeof(CLIENT_ID) * NUM_PLAYERS);
  memcpy(buffer.data(), characterAssignments, sizeof(CLIENT_ID) * NUM_PLAYERS);
  return buffer;
}

CharacterResponsePacket
CharacterResponsePacket::deserialize(const vector<char> &payload) {
  CLIENT_ID characterAssignments[NUM_PLAYERS];

  memcpy(characterAssignments, payload.data(), sizeof(CLIENT_ID) * NUM_PLAYERS);
  CharacterResponsePacket packet(characterAssignments);
  return packet;
}

vector<char> LevelChangePacket::serialize() const {
  vector<char> buffer(sizeof(LEVEL_ID));
  memcpy(buffer.data(), &level, sizeof(LEVEL_ID));
  return buffer;
}

LevelChangePacket LevelChangePacket::deserialize(const vector<char> &payload) {
  LEVEL_ID level;

  memcpy(&level, payload.data(), sizeof(LEVEL_ID));
  LevelChangePacket packet(level);
  return packet;
}

vector<char> ActivatePacket::serialize() const {
  vector<char> buffer(sizeof(OBJECT_ID));
  memcpy(buffer.data(), &id, sizeof(OBJECT_ID));
  return buffer;
}

ActivatePacket ActivatePacket::deserialize(const vector<char> &payload) {
  OBJECT_ID id;

  memcpy(&id, payload.data(), sizeof(OBJECT_ID));
  ActivatePacket packet(id);
  return packet;
}

vector<char> KeypadPacket::serialize() const {
  vector<char> buffer(sizeof(OBJECT_ID) + 2 * sizeof(bool));
  unsigned long size = 0;
  memcpy(buffer.data(), &id, sizeof(OBJECT_ID));
  size += sizeof(OBJECT_ID);
  memcpy(buffer.data() + size, &display, sizeof(bool));
  size += sizeof(bool);
  memcpy(buffer.data() + size, &unlocked, sizeof(bool));
  return buffer;
}

KeypadPacket KeypadPacket::deserialize(const vector<char> &payload) {
  OBJECT_ID id;
  bool display;
  bool unlocked;

  unsigned long size = 0;
  memcpy(&id, payload.data(), sizeof(OBJECT_ID));
  size += sizeof(OBJECT_ID);
  memcpy(&display, payload.data() + size, sizeof(bool));
  size += sizeof(bool);
  memcpy(&unlocked, payload.data() + size, sizeof(bool));
  KeypadPacket packet(id, display, unlocked);
  return packet;
}

vector<char> MovementPacket::serialize() const {
  vector<char> buffer(sizeof(PLAYER_ID) + sizeof(MovementType) +
                      sizeof(glm::vec3));
  unsigned long size = 0;
  memcpy(buffer.data(), &id, sizeof(PLAYER_ID));
  size += sizeof(PLAYER_ID);
  memcpy(buffer.data() + size, &movementType, sizeof(MovementType));
  return buffer;
}

MovementPacket MovementPacket::deserialize(const vector<char> &payload) {
  PLAYER_ID id;
  MovementType movementType;

  unsigned long size = 0;
  memcpy(&id, payload.data(), sizeof(PLAYER_ID));
  size += sizeof(PLAYER_ID);
  memcpy(&movementType, payload.data() + size, sizeof(MovementType));
  MovementPacket packet(id, movementType);
  return packet;
}

vector<char> RotationPacket::serialize() const {
  vector<char> buffer(sizeof(PLAYER_ID) + sizeof(glm::vec3));
  unsigned long size = 0;
  memcpy(buffer.data(), &id, sizeof(PLAYER_ID));
  size += sizeof(PLAYER_ID);
  serializeVector(buffer.data(), rotation, size);
  return buffer;
}

RotationPacket RotationPacket::deserialize(const vector<char> &payload) {
  PLAYER_ID id;
  glm::vec3 rotation;

  unsigned long size = 0;
  memcpy(&id, payload.data(), sizeof(PLAYER_ID));
  size += sizeof(PLAYER_ID);
  rotation = deserializeVector(payload, size);
  RotationPacket packet(id, rotation);
  return packet;
}

vector<char> InteractionPacket::serialize() const {
  vector<char> buffer(sizeof(PLAYER_ID));
  unsigned long size = 0;
  memcpy(buffer.data(), &id, sizeof(PLAYER_ID));
  return buffer;
}

InteractionPacket InteractionPacket::deserialize(const vector<char> &payload) {
  PLAYER_ID id;

  unsigned long size = 0;
  memcpy(&id, payload.data(), sizeof(PLAYER_ID));
  InteractionPacket packet(id);
  return packet;
}

vector<char> CharacterSelectPacket::serialize() const {
  vector<char> buffer(sizeof(PLAYER_ID) + sizeof(CLIENT_ID));
  unsigned long size = 0;
  memcpy(buffer.data(), &playerID, sizeof(PLAYER_ID));
  size += sizeof(PLAYER_ID);
  memcpy(buffer.data() + size, &clientID, sizeof(CLIENT_ID));
  return buffer;
}

CharacterSelectPacket
CharacterSelectPacket::deserialize(const vector<char> &payload) {
  PLAYER_ID playerID;
  CLIENT_ID clientID;

  unsigned long size = 0;
  memcpy(&playerID, payload.data(), sizeof(PLAYER_ID));
  size += sizeof(PLAYER_ID);
  memcpy(&clientID, payload.data() + size, sizeof(CLIENT_ID));
  CharacterSelectPacket packet(playerID, clientID);
  return packet;
}

vector<char> KeypadInputPacket::serialize() const {
  vector<char> buffer(sizeof(OBJECT_ID) + sizeof(CLIENT_ID) +
                      inputSequence.size() * sizeof(int) + sizeof(bool));
  unsigned long size = 0;
  memcpy(buffer.data(), &objectID, sizeof(OBJECT_ID));
  size += sizeof(OBJECT_ID);
  memcpy(buffer.data() + size, &clientID, sizeof(CLIENT_ID));
  size += sizeof(CLIENT_ID);
  for (int input : inputSequence) {
    memcpy(buffer.data() + size, &input, sizeof(int));
    size += sizeof(int);
  }
  memcpy(buffer.data() + size, &close, sizeof(bool));
  return buffer;
}

KeypadInputPacket KeypadInputPacket::deserialize(const vector<char> &payload) {
  OBJECT_ID objectID;
  CLIENT_ID clientID;
  vector<int> inputSequence;
  bool close;

  unsigned long size = 0;
  memcpy(&objectID, payload.data(), sizeof(OBJECT_ID));
  size += sizeof(OBJECT_ID);
  memcpy(&clientID, payload.data() + size, sizeof(CLIENT_ID));
  size += sizeof(CLIENT_ID);
  for (int i = 0; i < 4; ++i) {
    int input;
    memcpy(&input, payload.data() + size, sizeof(int));
    inputSequence.push_back(input);
    size += sizeof(int);
  }
  memcpy(&close, payload.data() + size, sizeof(bool));
  KeypadInputPacket packet(objectID, clientID, inputSequence, close);
  return packet;
}

vector<char> DisconnectPacket::serialize() const {
  vector<char> buffer(sizeof(CLIENT_ID));
  memcpy(buffer.data(), &id, sizeof(CLIENT_ID));
  return buffer;
}

DisconnectPacket DisconnectPacket::deserialize(const vector<char> &payload) {
  CLIENT_ID id;

  memcpy(&id, payload.data(), sizeof(CLIENT_ID));
  DisconnectPacket packet(id);
  return packet;
}

unique_ptr<IPacket> deserialize(PacketType type, vector<char> &payload) {
  switch (type) {
  case PacketType::INIT:
    return make_unique<InitPacket>(InitPacket::deserialize(payload));
  case PacketType::OBJECT:
    return make_unique<ObjectPacket>(ObjectPacket::deserialize(payload));
  case PacketType::GAMESTATE:
    return make_unique<GameStatePacket>(GameStatePacket::deserialize(payload));
  case PacketType::CHARACTERRESPONSE:
    return make_unique<CharacterResponsePacket>(
        CharacterResponsePacket::deserialize(payload));
  case PacketType::LEVELCHANGE:
    return make_unique<LevelChangePacket>(
        LevelChangePacket::deserialize(payload));
  case PacketType::ACTIVATE:
    return make_unique<ActivatePacket>(ActivatePacket::deserialize(payload));
  case PacketType::KEYPAD:
    return make_unique<KeypadPacket>(KeypadPacket::deserialize(payload));
  case PacketType::MOVEMENT:
    return make_unique<MovementPacket>(MovementPacket::deserialize(payload));
  case PacketType::ROTATION:
    return make_unique<RotationPacket>(RotationPacket::deserialize(payload));
  case PacketType::INTERACTION:
    return make_unique<InteractionPacket>(
        InteractionPacket::deserialize(payload));
  case PacketType::CHARACTERSELECT:
    return make_unique<CharacterSelectPacket>(
        CharacterSelectPacket::deserialize(payload));
  case PacketType::KEYPADINPUT:
    return make_unique<KeypadInputPacket>(
        KeypadInputPacket::deserialize(payload));
  case PacketType::DISCONNECT:
    return make_unique<DisconnectPacket>(
        DisconnectPacket::deserialize(payload));
  default:
    throw runtime_error("Unknown packet type");
  }
}