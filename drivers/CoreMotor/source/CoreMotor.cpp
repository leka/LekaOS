// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreMotor.h"

namespace leka {

using namespace mbed;

void CoreMotor::spin(rotation_t rotation, float speed)
{
	if (rotation == Rotation::clockwise) {
		setDirections(1, 0);
	} else {
		setDirections(0, 1);
	}

	setSpeed(speed);
}

void CoreMotor::stop()
{
	setDirections(0, 0);
	setSpeed(0);
	_speed.suspend();
}

void CoreMotor::setDirections(int dir_1, int dir_2)
{
	_dir_1.write(dir_1);
	_dir_2.write(dir_2);
}

void CoreMotor::setSpeed(float speed)
{
	// TODO (@ladislas) - add tests
	if (speed < 0.0F) {
		_speed.write(0);
		_speed.suspend();

	} else if (speed > 1.0F) {
		_speed.resume();
		_speed.write(1.0F);

	} else {
		_speed.resume();
		_speed.write(speed);
	}
}

}	// namespace leka
