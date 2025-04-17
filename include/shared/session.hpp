#pragma once
#include "packets.hpp"
#include <stdint.h>
#include <vector>

// enum class ConnectionState {
//     DISCONNECTED,
//     CONNECTING,
//     CONNECTED,
//     ERROR
// };

class Session {
    public:
        void process_packets(PacketType type, vector<char> payload, uint16_t size);
};