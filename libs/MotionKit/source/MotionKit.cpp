// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "MotionKit.hpp"

#include "ThisThread.h"

using namespace leka;
using namespace std::chrono_literals;

void MotionKit::stop()
{
	_timeout.stop();
	_motor_left.stop();
	_motor_right.stop();

	_imukit.onEulerAnglesReady({});

	_target_not_reached		  = false;
	_rotate_x_turns_requested = false;
}

void MotionKit::startYawRotation(float degrees, Rotation direction,
								 const std::function<void()> &on_rotation_ended_callback)
{
	stop();

	auto starting_angle = _imukit.getEulerAngles();
	_rotation_control.setTarget(starting_angle, degrees);

	_target_not_reached		  = true;
	_rotate_x_turns_requested = true;

	auto on_timeout = [this] { stop(); };

	_timeout.onTimeout(on_timeout);
	_timeout.start(10s);

	auto on_euler_angles_rdy_callback = [this, direction](const EulerAngles &euler_angles) {
		processAngleForRotation(euler_angles, direction);
	};

	_imukit.onEulerAnglesReady(on_euler_angles_rdy_callback);

	_on_rotation_ended_callback = on_rotation_ended_callback;
}

// LCOV_EXCL_START - Dynamic behavior, involving motors and time.
void MotionKit::processAngleForRotation(const EulerAngles &angles, Rotation direction)
{
	auto must_stop = [&] { return !_rotate_x_turns_requested && !_target_not_reached; };

	if (must_stop()) {
		stop();

		if (_on_rotation_ended_callback) {
			_on_rotation_ended_callback();
		}

		return;
	}

	if (_rotate_x_turns_requested && _target_not_reached) {
		auto speed = _rotation_control.processRotationAngle(angles);

		setMotorsSpeedAndDirection(speed, direction);
	}
}

void MotionKit::setMotorsSpeedAndDirection(float speed, Rotation direction)
{
	if (speed == 0.F) {
		_motor_left.stop();
		_motor_right.stop();
		_target_not_reached		  = false;
		_rotate_x_turns_requested = false;
	} else {
		_motor_left.spin(direction, speed);
		_motor_right.spin(direction, speed);
		_target_not_reached = true;
	}
}
// LCOV_EXCL_STOP
