// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "MotionKit.hpp"

#include "MathUtils.h"
#include "ThisThread.h"

using namespace leka;
using namespace std::chrono_literals;

void MotionKit::stop()
{
	_timeout.stop();
	_motor_left.stop();
	_motor_right.stop();

	_stabilisation_requested  = false;
	_target_not_reached		  = false;
	_rotate_x_turns_requested = false;
}

void MotionKit::rotate(uint8_t number_of_rotations, Rotation direction,
					   const std::function<void()> &on_rotation_ended_callback)
{
	stop();

	_euler_angles_previous = _imukit.getEulerAngles();
	_pid.setTargetYaw(_euler_angles_previous.yaw);

	_target_not_reached		  = true;
	_stabilisation_requested  = false;
	_rotate_x_turns_requested = true;

	_angle_rotation_sum	   = 0;
	_angle_rotation_target = static_cast<float>(number_of_rotations) * 360.F;

	auto on_timeout = [this] { stop(); };

	_timeout.onTimeout(on_timeout);
	_timeout.start(10s);

	auto on_euler_angles_rdy_callback = [this, direction](const interface::EulerAngles &euler_angles) {
		iterateOverRotation(euler_angles, direction);
	};

	_imukit.onEulerAnglesReady(on_euler_angles_rdy_callback);

	_on_rotation_ended_callback = on_rotation_ended_callback;
}

void MotionKit::startStabilisation()
{
	stop();

	_euler_angles_previous = _imukit.getEulerAngles();
	_pid.setTargetYaw(_euler_angles_previous.yaw);

	_target_not_reached		  = false;
	_stabilisation_requested  = true;
	_rotate_x_turns_requested = false;

	auto on_euler_angles_rdy_callback = [this](const interface::EulerAngles &euler_angles) {
		iterateOverRotation(euler_angles);
	};

	_imukit.onEulerAnglesReady(on_euler_angles_rdy_callback);
}

// LCOV_EXCL_START - Dynamic behavior, involving motors and time.
void MotionKit::iterateOverRotation(const interface::EulerAngles &angles, Rotation direction)
{
	auto must_stop = [&] { return !_stabilisation_requested && !_rotate_x_turns_requested && !_target_not_reached; };

	if (must_stop()) {
		stop();
		_imukit.onEulerAnglesReady({});

		if (_on_rotation_ended_callback) {
			_on_rotation_ended_callback();
		}

		return;
	}

	auto counting_rotations = [&](auto current_yaw) {
		if (auto abs_yaw_delta = std::abs(_euler_angles_previous.yaw - current_yaw); abs_yaw_delta >= 300.F) {
			_angle_rotation_sum += 360.F - abs_yaw_delta;
		} else {
			_angle_rotation_sum += abs_yaw_delta;
		}
		_euler_angles_previous.yaw = current_yaw;
	};

	counting_rotations(angles.yaw);

	if (_stabilisation_requested && _target_not_reached) {
		auto [speed, rotation] = _pid.processPID(angles.pitch, angles.roll, angles.yaw);

		executeSpeed(speed, rotation);
	}

	// Without regulation
	// if (_rotate_x_turns_requested && _target_not_reached) {
	// 	if (auto error_position_current = _angle_rotation_target - _angle_rotation_sum;
	// 		error_position_current > minimum_viable_position_error) {
	// 		return;
	// 	}
	// 	_target_not_reached		  = false;
	// 	_rotate_x_turns_requested = false;
	// }

	// With regulation
	if (_rotate_x_turns_requested && _target_not_reached) {
		auto speed = _pid.processPIDByError(_angle_rotation_target - _angle_rotation_sum);

		executeSpeed(speed, direction);
	}
}

auto MotionKit::mapSpeed(float speed) const -> float
{
	return utils::math::map(speed, 0.F, kEntryLimitSpeed, kMinimalViableRobotPwm, kPwmMaxValue);
}

void MotionKit::executeSpeed(float speed, Rotation direction)
{
	auto speed_bounded = mapSpeed(speed);
	if (speed_bounded <= kMinimalViableRobotPwm + kEpsilon) {
		_motor_left.stop();
		_motor_right.stop();
		_target_not_reached		  = false;
		_rotate_x_turns_requested = false;
	} else {
		_motor_left.spin(direction, speed_bounded);
		_motor_right.spin(direction, speed_bounded);
		_target_not_reached = true;
	}
}
// LCOV_EXCL_STOP
