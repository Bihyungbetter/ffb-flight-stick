#include <unity.h>

#include "mixing.h"

using namespace ffb;

void test_mix_pure_pitch() {
    MotorTorques m = mixTorques(AxisTorques{2.0f, 0.0f});
    TEST_ASSERT_EQUAL_FLOAT(1.0f, m.m0);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, m.m1);
}

void test_mix_pure_roll() {
    MotorTorques m = mixTorques(AxisTorques{0.0f, 2.0f});
    TEST_ASSERT_EQUAL_FLOAT(1.0f, m.m0);
    TEST_ASSERT_EQUAL_FLOAT(-1.0f, m.m1);
}

void test_unmix_pure_sum_is_pitch() {
    AxisAngles a = unmixAngles(MotorAngles{1.0f, 1.0f});
    TEST_ASSERT_EQUAL_FLOAT(1.0f, a.pitch);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, a.roll);
}

void test_unmix_pure_difference_is_roll() {
    AxisAngles a = unmixAngles(MotorAngles{1.0f, -1.0f});
    TEST_ASSERT_EQUAL_FLOAT(0.0f, a.pitch);
    TEST_ASSERT_EQUAL_FLOAT(1.0f, a.roll);
}

void test_power_conservation() {
    AxisTorques axis{3.0f, -1.5f};
    MotorTorques tau = mixTorques(axis);

    MotorAngles motorVelocity{0.4f, -0.9f};
    AxisAngles axisVelocity = unmixAngles(motorVelocity);

    float motorPower = tau.m0 * motorVelocity.m0 + tau.m1 * motorVelocity.m1;
    float axisPower = axis.pitch * axisVelocity.pitch + axis.roll * axisVelocity.roll;

    TEST_ASSERT_FLOAT_WITHIN(1e-5f, axisPower, motorPower);
}

int main(int argc, char** argv) {
    UNITY_BEGIN();
    RUN_TEST(test_mix_pure_pitch);
    RUN_TEST(test_mix_pure_roll);
    RUN_TEST(test_unmix_pure_sum_is_pitch);
    RUN_TEST(test_unmix_pure_difference_is_roll);
    RUN_TEST(test_power_conservation);
    return UNITY_END();
}
