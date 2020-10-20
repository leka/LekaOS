// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "Motor.h"

namespace leka {

using namespace mbed;

Motor::Motor(DigitalOut &direction_1, DigitalOut &direction_2, PwmOut &speed)
	: _direction_1(direction_1), _direction_2(direction_2), _speed(speed)
{
	// nothing to do
	// _speed.period_us(500.0)	  // Cycle of 500us = 1/2kHz
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

void Motor::brake(void)
{
	_direction_1.write(0);
	_direction_2.write(0);
}

}	// namespace leka
