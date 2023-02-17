// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PID.hpp"
#include <algorithm>

using namespace leka;

void PID::setTargetYaw(float angle)
{
	_target_yaw = angle;
}

auto PID::processPID([[maybe_unused]] float pitch, [[maybe_unused]] float roll, float yaw)
	-> std::tuple<float, Rotation>
{
	auto direction = Rotation {};

	auto speed = processPIDByError(_target_yaw - yaw);

	if (speed < 0) {
		speed	  = -speed;
		direction = Rotation::counterClockwise;
	} else {
		direction = Rotation::clockwise;
	}

	return {speed, direction};
}

auto PID::processPIDByError(float error_position_current) -> float
{
	if (std::abs(error_position_current) < kStaticBound) {
		_error_position_total += error_position_current;
		_error_position_total = std::min(_error_position_total, 50.F / Parameters::Ki);
	} else {
		_error_position_total = 0.F;
	}
	if (std::abs(error_position_current) < kStaticBound) {
		_derivative = 0.F;
	}

	_proportional = error_position_current * Parameters::Kp;
	_integral	  = _error_position_total * Parameters::Ki;
	_derivative	  = (error_position_current - _error_position_last) * Parameters::Kd;

	_error_position_last = error_position_current;

	auto speed = (_proportional + _integral + _derivative) / kDeltaT;

	return speed;
}
