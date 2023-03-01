// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "RotationControl.hpp"
#include <algorithm>

#include "MathUtils.h"

using namespace leka;

auto RotationControl::processRotationAngle(float target, float current) -> float
{
	auto error_position_current = target - current;

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

	return mapSpeed(speed);
}

auto RotationControl::calculateYawRotation(float previous_yaw, float yaw) -> float
{
	auto abs_yaw_delta = std::abs(previous_yaw - yaw);
	if (abs_yaw_delta >= 300.F) {
		return 360.F - abs_yaw_delta;
	}
	return abs_yaw_delta;
}

auto RotationControl::mapSpeed(float speed) const -> float
{
	auto bounded_speed = utils::math::map(speed, 0.F, kInputSpeedLimit, kMinimalViableRobotPwm, kPwmMaxValue);
	if (bounded_speed < kMinimalViableRobotPwm + kEpsilon) {
		return 0.0;
	}
	return bounded_speed;
}
