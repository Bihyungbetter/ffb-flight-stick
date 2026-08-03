#include "uart_link.h"

#include <string.h>

namespace ffb {

UartLink::UartLink(uint8_t rxPin, uint8_t txPin, uint32_t baud)
    : serial_(rxPin, txPin), baud_(baud) {}

void UartLink::begin() {
    serial_.begin(baud_);
}

void UartLink::sendTarget(float torque) {
    CommandPacket pkt{};
    pkt.start = kFrameStart;
    pkt.target_torque = torque;
    pkt.checksum = xorChecksum(reinterpret_cast<uint8_t*>(&pkt), sizeof(pkt) - 1);
    serial_.write(reinterpret_cast<uint8_t*>(&pkt), sizeof(pkt));
}

bool UartLink::poll(Telemetry& out) {
    bool gotOne = false;
    while (serial_.available()) {
        uint8_t b = (uint8_t)serial_.read();
        if (have_ == 0 && b != kFrameStart) continue;
        buf_[have_++] = b;
        if (have_ == sizeof(buf_)) {
            TelemetryPacket pkt;
            memcpy(&pkt, buf_, sizeof(pkt));
            if (xorChecksum(buf_, sizeof(buf_) - 1) == pkt.checksum) {
                out.angle_rad = pkt.angle_rad;
                out.velocity_rad_s = pkt.velocity_rad_s;
                out.fault = pkt.fault;
                gotOne = true;
            }
            have_ = 0;
        }
    }
    return gotOne;
}

}
