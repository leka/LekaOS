// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <interface/drivers/Motor.h>
#include <interface/libs/EventLoop.h>

#include "RotationControl.hpp"
#include "StabilizationControl.hpp"
#include "interface/drivers/Timeout.h"
#include "interface/libs/IMUKit.hpp"

namespace leka {

class MotionKit
{
  public:
	MotionKit(interface::Motor &motor_left, interface::Motor &motor_right, interface::IMUKit &imu_kit,
			  interface::Timeout &timeout)
		: _motor_left(motor_left), _motor_right(motor_right), _imukit(imu_kit), _timeout(timeout)
	{
	}

	void rotate(float number_of_rotations, Rotation direction,
				const std::function<void()> &on_rotation_ended_callback = {});
	void startStabilisation();

	void stop();

  private:
	void processAngleForRotation(const EulerAngles &angles, Rotation direction);
	void processAngleForStabilization(const EulerAngles &angles);

	void calculateYawRotation(float yaw);
	void setMotorsSpeedAndDirection(float speed, Rotation direction);

	interface::Motor &_motor_left;
	interface::Motor &_motor_right;
	interface::IMUKit &_imukit;
	interface::Timeout &_timeout;

	RotationControl _rotation_control;
	StabilizationControl _stabilization_control;

	float _angle_rotation_target = 0.F;
	float _angle_rotation_sum	 = 0.F;
	std::function<void()> _on_rotation_ended_callback {};

	bool _target_not_reached	   = false;
	bool _rotate_x_turns_requested = false;

	EulerAngles _euler_angles_previous {};
	EulerAngles _target_angles {};
};

}	// namespace leka
