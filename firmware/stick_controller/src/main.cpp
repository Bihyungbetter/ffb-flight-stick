#include <Arduino.h>

#include "hid_joystick.h"
#include "mixing.h"
#include "uart_link.h"

// Master controller: reads shaft angle/velocity from both motor_node
// boards, un-mixes it into pitch/roll, runs a basic spring-to-center force
// model, mixes that back into per-motor torque commands, and reports
// pitch/roll to the PC as a standard USB joystick.
//
// This covers build-order stages 1-3 from the top-level README plus a
// minimal local stage 4: real closed-loop differential force feedback, but
// not yet a PC/sim-driven USB HID-PID effect engine (see
// firmware/README.md "What's not done yet"). A sim's native force cues will
// not reach this stick until that layer exists -- today it only self-centers.

using namespace ffb;

UartLink link0(PA9, PA10, 921600);   // to motor_node 0 -- confirm against wiring harness
UartLink link1(PC10, PC11, 921600);  // to motor_node 1 -- confirm against wiring harness

HidJoystick joystick;

// Spring-to-center gains -- start soft and increase gradually once the
// mechanism is bolted together; too stiff on a loose bench rig just
// oscillates. Units are whatever `target_torque` means to motor_node today
// (volts, in voltage-mode torque control -- see firmware/README.md).
constexpr float kSpringK = 0.6f;
constexpr float kDamperB = 0.02f;

// Normalizes angle to a HID axis value. TBD against the actual mechanical
// stop-to-stop travel once the gimbal is assembled.
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
        // Lost telemetry from a node this tick -- fail safe to zero torque
        // rather than commanding stale/last-known values.
        link0.sendTarget(0.0f);
        link1.sendTarget(0.0f);
    }

    joystick.update();
}
