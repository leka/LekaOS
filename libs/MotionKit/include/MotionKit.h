// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <interface/drivers/Motor.h>

#include "IMUKit.h"
#include "PID.h"
#include "interface/drivers/Timeout.h"

namespace leka {

class MotionKit
{
  public:
	MotionKit(interface::Motor &motor_left, interface::Motor &motor_right, IMUKit &imu_kit,
			  interface::EventLoop &event_loop)
		: _motor_left(motor_left), _motor_right(motor_right), _imukit(imu_kit), _event_loop(event_loop) {};

	void init();

	void rotate(uint8_t number_of_rotations, Rotation direction);
	void startStabilisation();

	void stop();

  private:
	void run();

	[[nodiscard]] auto mapSpeed(float speed) const -> float;
	void executeSpeed(float speed, Rotation direction);

	interface::Motor &_motor_left;
	interface::Motor &_motor_right;
	IMUKit &_imukit;
	interface::EventLoop &_event_loop;
	PID _pid;

	uint8_t _rotations_to_execute = 0;

	bool _target_not_reached	   = false;
	bool _stabilisation_requested  = false;
	bool _rotate_x_turns_requested = false;

	const float kReferenceAngle		   = 180.F;
	const float kMinimalViableRobotPwm = 0.25F;	  // ? Under this pwm value, torque is too low to spin the wheel
	const float kPwmMaxValue		   = 1.F;
	const float kPwmMarginLimit		   = 0.28F;
};

}	// namespace leka
