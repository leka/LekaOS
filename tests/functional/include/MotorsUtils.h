// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_SPIKE_MOTORS_UTILS_H_
#define _LEKA_OS_SPIKE_MOTORS_UTILS_H_

#include "LKCoreMotor.h"

leka::LKCoreMotor motor_right(PinName::MOTOR_RIGHT_DIRECTION_1, PinName::MOTOR_RIGHT_DIRECTION_2,
							  PinName::MOTOR_RIGHT_PWM);
leka::LKCoreMotor motor_left(PinName::MOTOR_LEFT_DIRECTION_1, PinName::MOTOR_LEFT_DIRECTION_2, PinName::MOTOR_LEFT_PWM);

void spinLeft()
{
	motor_right.spin(leka::Rotation::clockwise, 0.5f);
	motor_left.spin(leka::Rotation::clockwise, 0.5f);
}

void spinRight()
{
	motor_right.spin(leka::Rotation::counterClockwise, 0.5f);
	motor_left.spin(leka::Rotation::counterClockwise, 0.5f);
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
