// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LekaMotors.h"

Motors::Motors(MotorBase &motor_right, MotorBase &motor_left) : _motor_right(motor_right), _motor_left(motor_left)
{
	// nothing to do
}

void Motors::translate(translation_t translation, float speed)
{
	if (translation == Translation::forward) {
		_motor_right.spin(Rotation::clockwise, speed);
		_motor_left.spin(Rotation::counterClockwise, speed);
	} else {
		_motor_right.spin(Rotation::counterClockwise, speed);
		_motor_left.spin(Rotation::clockwise, speed);
	}
}

void Motors::spin(turn_t turn, float speed)
{
	if (turn == Turn::right) {
		_motor_right.spin(Rotation::counterClockwise, speed);
		_motor_left.spin(Rotation::counterClockwise, speed);
	} else {
		_motor_right.spin(Rotation::clockwise, speed);
		_motor_left.spin(Rotation::clockwise, speed);
	}
}

void Motors::stop(void)
{
	_motor_right.stop();
	_motor_left.stop();
}

void Motors::brake(void)
{
	_motor_right.brake();
	_motor_left.brake();
}
