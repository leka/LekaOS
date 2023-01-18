// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "MotionKit.h"

#include "MathUtils.h"
#include "ThisThread.h"

using namespace leka;
using namespace std::chrono_literals;

void MotionKit::init()
{
	_event_loop.registerCallback([this] { run(); });
}

void MotionKit::stop()
{
	_timeout.stop();
	_motor_left.stop();
	_motor_right.stop();
	_event_loop.stop();

	_stabilisation_requested  = false;
	_target_not_reached		  = false;
	_rotate_x_turns_requested = false;
}

void MotionKit::rotate(uint8_t number_of_rotations, Rotation direction,
					   const std::function<void()> &on_rotation_ended_callback)
{
	stop();

	_imukit.start();
	_imukit.setOrigin();

	_target_not_reached		  = true;
	_stabilisation_requested  = false;
	_rotate_x_turns_requested = true;

	_rotations_to_execute = number_of_rotations;

	_motor_left.spin(direction, kPwmMaxValue);
	_motor_right.spin(direction, kPwmMaxValue);

	auto on_timeout = [this] { stop(); };

	_timeout.onTimeout(on_timeout);
	_timeout.start(10s);

	_event_loop.start();

	_on_rotation_ended_callback = on_rotation_ended_callback;
}

void MotionKit::startStabilisation()
{
	stop();

	_imukit.start();
	_imukit.setOrigin();

	_target_not_reached		  = false;
	_stabilisation_requested  = true;
	_rotate_x_turns_requested = false;

	_event_loop.start();
}

// LCOV_EXCL_START - Dynamic behavior, involving motors and time.
void MotionKit::run()
{
	auto last_yaw			= kReferenceAngle;
	auto rotations_executed = 0;

	auto must_rotate = [&] { return _rotate_x_turns_requested && rotations_executed != _rotations_to_execute; };

	auto check_complete_rotations_executed = [&](auto current_yaw) {
		if (std::abs(last_yaw - current_yaw) >= 300.F) {
			++rotations_executed;
		}
	};

	while (must_rotate()) {
		auto [current_pitch, current_roll, current_yaw] = _imukit.getAngles();

		check_complete_rotations_executed(current_yaw);

		rtos::ThisThread::sleep_for(70ms);
		last_yaw = current_yaw;
	}

	_rotate_x_turns_requested = false;
	_rotations_to_execute	  = 0;

	while (_stabilisation_requested || _target_not_reached) {
		auto [pitch, roll, yaw] = _imukit.getAngles();
		auto [speed, rotation]	= _pid.processPID(pitch, roll, yaw);

		executeSpeed(speed, rotation);

		rtos::ThisThread::sleep_for(70ms);
	}

	if (_on_rotation_ended_callback != nullptr) {
		_on_rotation_ended_callback();
	}

	_imukit.stop();
}

auto MotionKit::mapSpeed(float speed) const -> float
{
	return utils::math::map(speed, 0.F, kPIDMaxValue, kMinimalViableRobotPwm, kPwmMaxValue);
}

void MotionKit::executeSpeed(float speed, Rotation direction)
{
	auto speed_bounded = mapSpeed(speed);
	if (speed_bounded <= kMinimalViableRobotPwm + kEpsilon) {
		_motor_left.stop();
		_motor_right.stop();
		_target_not_reached = false;
	} else {
		_motor_left.spin(direction, speed_bounded);
		_motor_right.spin(direction, speed_bounded);
		_target_not_reached = true;
	}
}
// LCOV_EXCL_STOP
