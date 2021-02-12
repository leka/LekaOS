// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_SPIKE_MOTORS_UTILS_H_
#define _LEKA_OS_SPIKE_MOTORS_UTILS_H_

#include "LKCoreMotor.h"

using namespace leka;

LKCoreMotor motor_right(MOTOR_RIGHT_DIRECTION_1, MOTOR_RIGHT_DIRECTION_2, MOTOR_RIGHT_PWM);
LKCoreMotor motor_left(MOTOR_LEFT_DIRECTION_1, MOTOR_LEFT_DIRECTION_2, MOTOR_LEFT_PWM);

void spinLeft()
{
	motor_right.spin(Rotation::clockwise, 0.5f);
	motor_left.spin(Rotation::clockwise, 0.5f);
}

void spinRight()
{
	motor_right.spin(Rotation::counterClockwise, 0.5f);
	motor_left.spin(Rotation::counterClockwise, 0.5f);
}

void stop()
{
	motor_right.stop();
	motor_left.stop();
}

void motor_thread()
{
	while (true) {
		spinLeft();
		rtos::ThisThread::sleep_for(5s);

		spinRight();
		rtos::ThisThread::sleep_for(5s);

		stop();
		rtos::ThisThread::sleep_for(5s);
	}
}

#endif	 // _LEKA_OS_SPIKE_MOTORS_UTILS_H_
