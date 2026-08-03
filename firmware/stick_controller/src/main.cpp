#include <Arduino.h>

#include "hid_joystick.h"
#include "mixing.h"
#include "uart_link.h"

using namespace ffb;

UartLink link0(PA9, PA10, 921600);
UartLink link1(PC10, PC11, 921600);

HidJoystick joystick;

constexpr float kSpringK = 0.6f;
constexpr float kDamperB = 0.02f;

constexpr float kAxisRangeRad = 0.6f;

AxisTorques computeSpring(AxisAngles angle, AxisAngles velocity) {
    return AxisTorques{
        -kSpringK * angle.pitch - kDamperB * velocity.pitch,
        -kSpringK * angle.roll - kDamperB * velocity.roll,
    };
}

void setup() {
    link0.begin();
    link1.begin();
    joystick.begin();
}

void loop() {
    Telemetry t0, t1;
    bool ok0 = link0.poll(t0);
    bool ok1 = link1.poll(t1);

    if (ok0 && ok1) {
        AxisAngles axisAngle = unmixAngles(MotorAngles{t0.angle_rad, t1.angle_rad});
        AxisAngles axisVel = unmixAngles(MotorAngles{t0.velocity_rad_s, t1.velocity_rad_s});

        AxisTorques axisTorque = computeSpring(axisAngle, axisVel);
        MotorTorques motorTorque = mixTorques(axisTorque);

        link0.sendTarget(motorTorque.m0);
        link1.sendTarget(motorTorque.m1);

        joystick.updateAxes(axisAngle.pitch / kAxisRangeRad, axisAngle.roll / kAxisRangeRad);
    } else {
        link0.sendTarget(0.0f);
        link1.sendTarget(0.0f);
    }

    joystick.update();
}
