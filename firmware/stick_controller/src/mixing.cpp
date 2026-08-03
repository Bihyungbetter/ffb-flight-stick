#include "mixing.h"

namespace ffb {

MotorTorques mixTorques(AxisTorques axis) {
    return MotorTorques{
        axis.pitch * 0.5f + axis.roll * 0.5f,
        axis.pitch * 0.5f - axis.roll * 0.5f,
    };
}

AxisAngles unmixAngles(MotorAngles motor) {
    return AxisAngles{
        (motor.m0 + motor.m1) * 0.5f,
        (motor.m0 - motor.m1) * 0.5f,
    };
}

}  // namespace ffb
