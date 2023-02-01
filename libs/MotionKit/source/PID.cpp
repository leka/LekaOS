// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PID.hpp"
#include <algorithm>

using namespace leka;

auto PID::processPID([[maybe_unused]] float pitch, [[maybe_unused]] float roll, float yaw)
	-> std::tuple<float, Rotation>
{
	auto direction			= Rotation {};
	_error_position_current = kTargetAngle - yaw;

	if (std::abs(_error_position_current) < kStaticBound) {
		_error_position_total += _error_position_current;
		_error_position_total = std::min(_error_position_total, 50.F / Parameters::Ki);
	} else {
		_error_position_total = 0.F;
	}
	if (std::abs(_error_position_current) < kStaticBound) {
		_derivative = 0.F;
	}

	_proportional = _error_position_current * Parameters::Kp;
	_integral	  = _error_position_total * Parameters::Ki;
	_derivative	  = (_error_position_current - _error_position_last) * Parameters::Kd;

	_error_position_last = _error_position_current;

	auto speed = (_proportional + _integral + _derivative) / kDeltaT;

	if (speed < 0) {
		speed	  = -speed;
		direction = Rotation::counterClockwise;
	} else {
		direction = Rotation::clockwise;
	}

	return {speed, direction};
}
