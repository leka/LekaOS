// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "RotationControl.hpp"
#include <algorithm>

#include "MathUtils.h"

using namespace leka;

void RotationControl::setTarget(EulerAngles starting_angle, float number_of_rotations)
{
	_euler_angles_previous = starting_angle;
	_angle_rotation_target = number_of_rotations * 360.F;
	_angle_rotation_sum	   = 0;
}

auto RotationControl::processRotationAngle(EulerAngles current_angles) -> float
{
	calculateTotalYawRotation(current_angles);

	auto error_position_current = _angle_rotation_target - _angle_rotation_sum;

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

void RotationControl::calculateTotalYawRotation(EulerAngles angle)
{
	if (auto abs_yaw_delta = std::abs(_euler_angles_previous.yaw - angle.yaw); abs_yaw_delta >= 300.F) {
		_angle_rotation_sum += 360.F - abs_yaw_delta;
	} else {
		_angle_rotation_sum += abs_yaw_delta;
	}
	_euler_angles_previous = angle;
}

auto RotationControl::mapSpeed(float speed) const -> float
{
	auto bounded_speed = utils::math::map(speed, 0.F, kInputSpeedLimit, kMinimalViableRobotPwm, kPwmMaxValue);
	if (bounded_speed < kMinimalViableRobotPwm + kEpsilon) {
		return 0.0;
	}
	return bounded_speed;
}
