#pragma once
#include <Arduino.h>

#include "../../common/protocol.h"

namespace ffb {

struct Telemetry {
    float angle_rad = 0;
    float velocity_rad_s = 0;
    uint8_t fault = 0;
};

class UartLink {
public:
    UartLink(uint8_t rxPin, uint8_t txPin, uint32_t baud);
    void begin();
    void sendTarget(float torque);
    bool poll(Telemetry& out);

private:
    HardwareSerial serial_;
    uint32_t baud_;
    uint8_t buf_[sizeof(TelemetryPacket)];
    size_t have_ = 0;
};

}
