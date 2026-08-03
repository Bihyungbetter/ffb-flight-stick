#include <Arduino.h>
#include <SimpleFOC.h>
#include <string.h>

#include "../../common/protocol.h"
#include "board_pins.h"

BLDCMotor motor = BLDCMotor(7);
BLDCDriver6PWM driver = BLDCDriver6PWM(PIN_PWM_UH, PIN_PWM_UL, PIN_PWM_VH, PIN_PWM_VL, PIN_PWM_WH,
                                       PIN_PWM_WL, PIN_ENABLE);
MagneticSensorSPI encoder = MagneticSensorSPI(AS5047_SPI, PIN_ENCODER_CS);

HardwareSerial linkSerial(UART_RX, UART_TX);

volatile float target_torque = 0.0f;

void applyCommand(const ffb::CommandPacket& pkt) {
    float t = pkt.target_torque;
    if (t > motor.voltage_limit) t = motor.voltage_limit;
    if (t < -motor.voltage_limit) t = -motor.voltage_limit;
    target_torque = t;
}

void pollLink() {
    static uint8_t buf[sizeof(ffb::CommandPacket)];
    static size_t have = 0;

    while (linkSerial.available()) {
        uint8_t b = (uint8_t)linkSerial.read();
        if (have == 0 && b != ffb::kFrameStart) continue;
        buf[have++] = b;
        if (have == sizeof(buf)) {
            ffb::CommandPacket pkt;
            memcpy(&pkt, buf, sizeof(pkt));
            if (ffb::xorChecksum(buf, sizeof(buf) - 1) == pkt.checksum) {
                applyCommand(pkt);
            }
            have = 0;
        }
    }
}

void sendTelemetry(uint8_t fault) {
    ffb::TelemetryPacket pkt{};
    pkt.start = ffb::kFrameStart;
    pkt.angle_rad = motor.shaft_angle;
    pkt.velocity_rad_s = motor.shaft_velocity;
    pkt.fault = fault;
    pkt.checksum = ffb::xorChecksum(reinterpret_cast<uint8_t*>(&pkt), sizeof(pkt) - 1);
    linkSerial.write(reinterpret_cast<uint8_t*>(&pkt), sizeof(pkt));
}

void setup() {
    pinMode(PIN_FAULT, INPUT);

    linkSerial.begin(UART_BAUD);

    driver.voltage_power_supply = 12;
    driver.init();
    motor.linkDriver(&driver);

    encoder.init();
    motor.linkSensor(&encoder);

    motor.torque_controller = TorqueControlType::voltage;
    motor.controller = MotionControlType::torque;
    motor.voltage_limit = 4.0f;

    motor.init();
    motor.initFOC();
}

void loop() {
    motor.loopFOC();
    motor.move(target_torque);

    pollLink();

    static uint32_t last_tx = 0;
    uint32_t now = micros();
    if (now - last_tx >= 1000) {
        uint8_t fault = digitalRead(PIN_FAULT) == LOW ? 1 : 0;
        sendTelemetry(fault);
        last_tx = now;
    }
}
