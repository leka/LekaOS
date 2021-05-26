// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreMotor.h"

#include "gtest/gtest.h"
#include "mocks/mock_DigitalOut.h"
#include "stubs/stub_PwmOut.h"

// Test list
// - create object - done
// - spin/direction logic
// - set speed
// - stop

using namespace leka;

auto dir_1 = mbed::DigitalOutMock {MOTOR_RIGHT_DIRECTION_1};
auto dir_2 = mbed::DigitalOutMock {MOTOR_RIGHT_DIRECTION_2};

CoreMotor motor(dir_1, dir_2, MOTOR_RIGHT_PWM);

TEST(CoreMotorTest, initialization)
{
	ASSERT_NE(&motor, nullptr);
}

TEST(CoreMotorTest, speedValueNotLowerThanZero)
{
	motor.spin(Rotation::clockwise, -100);

	auto status = motor.getStatus();

	ASSERT_EQ(spy_PwmOut_getValue(), 0);
	ASSERT_EQ(status.speed, 0);
}

TEST(CoreMotorTest, speedValueNotGreaterThanOne)
{
	motor.spin(Rotation::clockwise, 100);

	auto status = motor.getStatus();

	ASSERT_EQ(spy_PwmOut_getValue(), 1);
	ASSERT_EQ(status.speed, 1);
}

TEST(CoreMotorTest, directionValuesForClockwiseRotation)
{
	motor.spin(Rotation::clockwise, 1);

	auto status = motor.getStatus();

	ASSERT_EQ(status.dir_1, 1);
	ASSERT_EQ(status.dir_2, 0);
}

TEST(CoreMotorTest, speedValueForClockwiseRotation)
{
	motor.spin(Rotation::clockwise, 1);

	auto status = motor.getStatus();

	ASSERT_EQ(spy_PwmOut_getValue(), 1);
	ASSERT_EQ(status.speed, 1);
}

TEST(CoreMotorTest, directionValuesForCounterClockwiseRotation)
{
	motor.spin(Rotation::counterClockwise, 1);

	auto status = motor.getStatus();

	ASSERT_EQ(status.dir_1, 0);
	ASSERT_EQ(status.dir_2, 1);
}

TEST(CoreMotorTest, speedValueForCounterClockwiseRotation)
{
	motor.spin(Rotation::counterClockwise, 1);

	auto status = motor.getStatus();

	ASSERT_EQ(spy_PwmOut_getValue(), 1);
	ASSERT_EQ(status.speed, 1);
}

TEST(CoreMotorTest, directionValuesForStop)
{
	motor.stop();

	auto status = motor.getStatus();

	ASSERT_EQ(status.dir_1, 0);
	ASSERT_EQ(status.dir_2, 0);
}

TEST(CoreMotorTest, speedValueForStop)
{
	motor.stop();

	auto status = motor.getStatus();

	ASSERT_EQ(spy_PwmOut_getValue(), 0);
	ASSERT_EQ(status.speed, 0);
}
