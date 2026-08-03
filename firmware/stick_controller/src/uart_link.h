#pragma once
#include <Arduino.h>

#include "../../common/protocol.h"

namespace ffb {

struct Telemetry {
    float angle_rad = 0;
    float velocity_rad_s = 0;
    uint8_t fault = 0;
};

// One of these per motor_node. Owns the UART, frames outgoing torque
// commands, and de-frames incoming telemetry.
class UartLink {
public:
    UartLink(uint8_t rxPin, uint8_t txPin, uint32_t baud);
    void begin();
    void sendTarget(float torque);
    // Returns true if a checksum-valid telemetry packet was parsed since the
    // last call and writes it into `out`. `out` is left untouched otherwise.
    bool poll(Telemetry& out);

private:
    HardwareSerial serial_;
    uint32_t baud_;
    uint8_t buf_[sizeof(TelemetryPacket)];
    size_t have_ = 0;
};

}  // namespace ffb
