// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LekaMotors.h"

namespace leka {

using namespace mbed;

Motors::Motors(interface::Motor &motor_right, interface::Motor &motor_left)
	: _motor_right(motor_right), _motor_left(motor_left)
{
	// nothing to do
}

void Motors::move(direction_t direction, uint8_t speed)
{
	float speed_f = static_cast<float>(speed) / 255.0f;

	if (direction == Direction::forward) {
		_motor_right.spin(Rotation::clockwise, speed_f);
		_motor_left.spin(Rotation::counterClockwise, speed_f);
	} else {
		_motor_right.spin(Rotation::counterClockwise, speed_f);
		_motor_left.spin(Rotation::clockwise, speed_f);
	}
}

void Motors::spin(rotation_t rotation, uint8_t speed)
{
	float speed_f = static_cast<float>(speed) / 255.0f;

	if (rotation == Rotation::right) {
		_motor_right.spin(Rotation::counterClockwise, speed_f);
		_motor_left.spin(Rotation::counterClockwise, speed_f);
	} else {
		_motor_right.spin(Rotation::clockwise, speed_f);
		_motor_left.spin(Rotation::clockwise, speed_f);
	}
}

void Motors::stop(void)
{
	_motor_right.stop();
	_motor_left.stop();
}

}	// namespace leka
