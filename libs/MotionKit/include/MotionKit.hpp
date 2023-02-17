// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <interface/drivers/Motor.h>
#include <interface/libs/EventLoop.h>

#include "PID.hpp"
#include "interface/drivers/Timeout.h"
#include "interface/libs/IMUKit.h"

namespace leka {

class MotionKit
{
  public:
	MotionKit(interface::Motor &motor_left, interface::Motor &motor_right, interface::IMUKit &imu_kit,
			  interface::Timeout &timeout)
		: _motor_left(motor_left), _motor_right(motor_right), _imukit(imu_kit), _timeout(timeout)
	{
	}

	void rotate(uint8_t number_of_rotations, Rotation direction,
				const std::function<void()> &on_rotation_ended_callback = {});
	void startStabilisation();

	void stop();

  private:
	void iterateOverRotation(const interface::EulerAngles &angles, Rotation direction = {});

	[[nodiscard]] auto mapSpeed(float speed) const -> float;
	void executeSpeed(float speed, Rotation direction);

	interface::Motor &_motor_left;
	interface::Motor &_motor_right;
	interface::IMUKit &_imukit;
	interface::Timeout &_timeout;
	PID _pid;

	float _angle_rotation_target = 0;
	float _angle_rotation_sum	 = 0;
	std::function<void()> _on_rotation_ended_callback {};

	bool _target_not_reached	   = false;
	bool _stabilisation_requested  = false;
	bool _rotate_x_turns_requested = false;

	interface::EulerAngles _euler_angles_previous
	{
	};

	// ? Calculated with following PID parameters : ((270-180)*Kp + (360-180)*Kd)/KDeltaT
	const float kEntryLimitSpeed			  = 3.15F;
	const float minimum_viable_position_error = 20.F;

	// ? When the motor is stopped, PWM values under kMinimalViableRobotPwm are too low to generate enough torque for
	// ? the motor to start spinning ? At the same time, kMinimalViableRobotPwm needs to be the lowest possible to avoid
	// ? overshooting when the target is reached

	const float kMinimalViableRobotPwm = 0.15F;
	const float kPwmMaxValue		   = 1.F;
	const float kEpsilon			   = 0.005F;
};

}	// namespace leka
