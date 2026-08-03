#pragma once
#include <stddef.h>
#include <stdint.h>

namespace ffb {

constexpr uint8_t kFrameStart = 0xA5;

#pragma pack(push, 1)

struct CommandPacket {
    uint8_t start;
    float target_torque;
    uint8_t checksum;
};

struct TelemetryPacket {
    uint8_t start;
    float angle_rad;
    float velocity_rad_s;
    uint8_t fault;
    uint8_t checksum;
};

#pragma pack(pop)

inline uint8_t xorChecksum(const uint8_t* data, size_t len) {
    uint8_t c = 0;
    for (size_t i = 0; i < len; i++) c ^= data[i];
    return c;
}

}
