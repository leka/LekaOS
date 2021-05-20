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

void CoreMotor::stop(void)
{
	setDirections(0, 0);
	setSpeed(0);
}

CoreMotor::Status CoreMotor::getStatus(void) const
{
	return _status;
}

void CoreMotor::setDirections(int dir_1, int dir_2)
{
	_dir_1.write(dir_1);
	_status.dir_1 = dir_1;

	_dir_2.write(dir_2);
	_status.dir_2 = dir_2;
}

void CoreMotor::setSpeed(float speed)
{
	if (speed < 0.0f) {
		_speed.write(0);
		_status.speed = 0;

	} else if (speed > 1.0f) {
		_speed.write(1.0f);
		_status.speed = 1.0f;
	} else {
		_speed.write(speed);
		_status.speed = speed;
	}
}

}	// namespace leka
