// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <tuple>

#include "CoreMotor.h"

struct Motors {
	leka::CoreMotor &left;
	leka::CoreMotor &right;
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
