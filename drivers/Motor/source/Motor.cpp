// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "Motor.h"

using namespace mbed;

Motor::Motor(DigitalOut &direction_1, DigitalOut &direction_2, PwmOut &speed)
	: _direction_1(direction_1), _direction_2(direction_2), _speed(speed)
{
	// nothing to do
}

void Motor::spin(rotation_t rotation, float speed)
{
	if (rotation == Rotation::clockwise) {
		_direction_1.write(1);
		_direction_2.write(0);
	} else {
		_direction_1.write(0);
		_direction_2.write(1);
	}

	_speed.write(speed);
}

void Motor::stop(void)
{
	spin(Rotation::clockwise, 0);
}
