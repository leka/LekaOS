// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_SPIKE_MOTORS_UTILS_H_
#define _LEKA_OS_SPIKE_MOTORS_UTILS_H_

#include <tuple>

#include "LKCoreMotor.h"

struct Motors {
	leka::LKCoreMotor &left;
	leka::LKCoreMotor &right;
};

void spinLeft(Motors &motors)
{
	motors.left.spin(leka::Rotation::clockwise, 0.5f);
	motors.right.spin(leka::Rotation::clockwise, 0.5f);
}

void spinRight(Motors &motors)
{
	motors.left.spin(leka::Rotation::counterClockwise, 0.5f);
	motors.right.spin(leka::Rotation::counterClockwise, 0.5f);
}

void stop(Motors &motors)
{
	motors.left.stop();
	motors.right.stop();
}

void motor_thread(Motors *motors)
{
	while (true) {
		spinLeft(*motors);
		rtos::ThisThread::sleep_for(5s);

		spinRight(*motors);
		rtos::ThisThread::sleep_for(5s);

		stop(*motors);
		rtos::ThisThread::sleep_for(5s);
	}
}

#endif	 // _LEKA_OS_SPIKE_MOTORS_UTILS_H_
