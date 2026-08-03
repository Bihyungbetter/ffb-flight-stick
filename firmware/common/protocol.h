#pragma once
#include <stddef.h>
#include <stdint.h>

// Fixed-size binary link between the stick_controller (master) and each
// motor_node (one B-G431B-ESC1 per motor). One packet each direction per
// control-loop tick. Framed with a start byte + checksum so a bit glitch on
// the UART can't be mistaken for a valid packet and drive real torque off
// garbage data.

namespace ffb {

constexpr uint8_t kFrameStart = 0xA5;

#pragma pack(push, 1)

// master -> motor_node
struct CommandPacket {
    uint8_t start;          // kFrameStart
    float target_torque;    // signed, arbitrary unit until current-mode FOC is wired up (see board_pins.h)
    uint8_t checksum;        // XOR of all preceding bytes
};

// motor_node -> master
struct TelemetryPacket {
    uint8_t start;           // kFrameStart
    float angle_rad;         // shaft angle, unwrapped, radians
    float velocity_rad_s;
    uint8_t fault;            // 0 = ok, nonzero = driver reports a fault; master must treat motor as untorqued
    uint8_t checksum;
};

#pragma pack(pop)

inline uint8_t xorChecksum(const uint8_t* data, size_t len) {
    uint8_t c = 0;
    for (size_t i = 0; i < len; i++) c ^= data[i];
    return c;
}

}  // namespace ffb
