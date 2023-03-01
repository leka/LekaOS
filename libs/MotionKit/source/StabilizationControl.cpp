// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "StabilizationControl.hpp"
#include <algorithm>

#include "MathUtils.h"

using namespace leka;

auto StabilizationControl::processStabilizationAngle(EulerAngles target, EulerAngles current)
	-> std::tuple<float, Rotation>
{
	auto direction = Rotation {};

	auto error_position_current = target.yaw - current.yaw;

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

	if (speed < 0) {
		speed	  = -speed;
		direction = Rotation::counterClockwise;
	} else {
		direction = Rotation::clockwise;
	}

	return {mapSpeed(speed), direction};
}

auto StabilizationControl::mapSpeed(float speed) const -> float
{
	auto bounded_speed = utils::math::map(speed, 0.F, kInputSpeedLimit, kMinimalViableRobotPwm, kPwmMaxValue);
	if (bounded_speed < kMinimalViableRobotPwm + kEpsilon) {
		return 0.F;
	}
	return bounded_speed;
}
