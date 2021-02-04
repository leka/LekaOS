// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreMotor.h"

#include "gtest/gtest.h"
#include "stub_PwmOut.h"

// Test list
// - create object - done
// - spin/direction logic
// - set speed
// - stop

using namespace leka;

LKCoreMotor motor(MOTOR_RIGHT_DIRECTION_1, MOTOR_RIGHT_DIRECTION_2, MOTOR_RIGHT_PWM);

TEST(LKCoreMotorTest, initialization)
{
	ASSERT_NE(&motor, nullptr);
}

TEST(LKCoreMotorTest, speedValueNotLowerThanZero)
{
	motor.spin(Rotation::clockwise, -100);

	auto status = motor.getStatus();

	ASSERT_EQ(spy_PwmOut_getValue(), 0);
	ASSERT_EQ(status.speed, 0);
}

TEST(LKCoreMotorTest, speedValueNotGreaterThanOne)
{
	motor.spin(Rotation::clockwise, 100);

	auto status = motor.getStatus();

	ASSERT_EQ(spy_PwmOut_getValue(), 1);
	ASSERT_EQ(status.speed, 1);
}

TEST(LKCoreMotorTest, directionValuesForClockwiseRotation)
{
	motor.spin(Rotation::clockwise, 1);

	auto status = motor.getStatus();

	ASSERT_EQ(status.dir_1, 1);
	ASSERT_EQ(status.dir_2, 0);
}

TEST(LKCoreMotorTest, speedValueForClockwiseRotation)
{
	motor.spin(Rotation::clockwise, 1);

	auto status = motor.getStatus();

	ASSERT_EQ(spy_PwmOut_getValue(), 1);
	ASSERT_EQ(status.speed, 1);
}

TEST(LKCoreMotorTest, directionValuesForCounterClockwiseRotation)
{
	motor.spin(Rotation::counterClockwise, 1);

	auto status = motor.getStatus();

	ASSERT_EQ(status.dir_1, 0);
	ASSERT_EQ(status.dir_2, 1);
}

TEST(LKCoreMotorTest, speedValueForCounterClockwiseRotation)
{
	motor.spin(Rotation::counterClockwise, 1);

	auto status = motor.getStatus();

	ASSERT_EQ(spy_PwmOut_getValue(), 1);
	ASSERT_EQ(status.speed, 1);
}

TEST(LKCoreMotorTest, directionValuesForStop)
{
	motor.stop();

	auto status = motor.getStatus();

	ASSERT_EQ(status.dir_1, 0);
	ASSERT_EQ(status.dir_2, 0);
}

TEST(LKCoreMotorTest, speedValueForStop)
{
	motor.stop();

	auto status = motor.getStatus();

	ASSERT_EQ(spy_PwmOut_getValue(), 0);
	ASSERT_EQ(status.speed, 0);
}
