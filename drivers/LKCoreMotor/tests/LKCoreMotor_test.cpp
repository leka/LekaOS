// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreMotor.h"

#include "gtest/gtest.h"

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
