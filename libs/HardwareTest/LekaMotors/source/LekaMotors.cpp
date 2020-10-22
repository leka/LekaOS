// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LekaMotors.h"

Motors::Motors(MotorBase &motor_right, MotorBase &motor_left) : _motor_right(motor_right), _motor_left(motor_left)
{
	// nothing to do
}

void Motors::move(direction_t direction, uint8_t speed)
{
	float float_speed = float(speed) / 255;

	if (direction == Direction::forward) {
		_motor_right.spin(Rotation::clockwise, float_speed);
		_motor_left.spin(Rotation::counterClockwise, float_speed);
	} else {
		_motor_right.spin(Rotation::counterClockwise, float_speed);
		_motor_left.spin(Rotation::clockwise, float_speed);
	}
}

void Motors::spin(rotation_t rotation, uint8_t speed)
{
	float float_speed = float(speed) / 255;

	if (spin == Rotation::right) {
		_motor_right.spin(Rotation::counterClockwise, float_speed);
		_motor_left.spin(Rotation::counterClockwise, float_speed);
	} else {
		_motor_right.spin(Rotation::clockwise, float_speed);
		_motor_left.spin(Rotation::clockwise, float_speed);
	}
}

void Motors::stop(void)
{
	_motor_right.stop();
	_motor_left.stop();
}
