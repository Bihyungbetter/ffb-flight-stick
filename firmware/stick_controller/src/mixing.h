#pragma once

// Pure math, no Arduino/hardware dependency -- kept host-testable (see
// firmware/stick_controller/test/test_mixing).

namespace ffb {

struct AxisTorques {
    float pitch;
    float roll;
};

struct MotorTorques {
    float m0;
    float m1;
};

struct AxisAngles {
    float pitch;
    float roll;
};

struct MotorAngles {
    float m0;
    float m1;
};

// Differential mixing: both motors drive both axes (see top-level README,
// "Differential mixing"). Sign/scale vs. the real gear ratio is TBD --
// tune here once the mechanism is built, everything else consumes these
// two functions and doesn't need to know about the differential.
MotorTorques mixTorques(AxisTorques axis);
AxisAngles unmixAngles(MotorAngles motor);

}  // namespace ffb
