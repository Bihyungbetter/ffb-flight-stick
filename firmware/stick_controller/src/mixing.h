#pragma once

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

MotorTorques mixTorques(AxisTorques axis);
AxisAngles unmixAngles(MotorAngles motor);

}
