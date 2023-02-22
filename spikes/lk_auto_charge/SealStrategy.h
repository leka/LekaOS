// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/drivers/Battery.h"
#include "interface/drivers/Motor.h"
#include "interface/drivers/Timeout.h"
#include "interface/libs/EventLoop.h"
#include "interface/libs/IMUKit.hpp"

namespace leka {

class SealStrategy
{
  public:
	SealStrategy(interface::EventLoop &event_loop, interface::Timeout &timeout, interface::Battery &battery,
				 interface::Motor &motor_left, interface::Motor &motor_right, interface::IMUKit &imu_kit)
		: _event_loop(event_loop),
		  _timeout(timeout),
		  _battery(battery),
		  _motor_left(motor_left),
		  _motor_right(motor_right),
		  _imukit(imu_kit)
	{
	}

	~SealStrategy() = default;

	void start();
	void stop();

  private:
	[[nodiscard]] auto convertToPwmFrom(float angle) const -> float;

	void run();

	void stopMotors();
	void spinRight(float left_speed, float right_speed);
	void spinLeft(float left_speed, float right_speed);
	void moveForward(float speed);
	void moveBackward(float speed);

	interface::EventLoop &_event_loop;
	interface::Timeout &_timeout;

	interface::Battery &_battery;
	interface::Motor &_motor_left;
	interface::Motor &_motor_right;
	interface::IMUKit &_imukit;

	const float kMinAngleInput = 0.F;
	const float kMaxAngleInput = 30.F;
	const float kMinPwmOutput  = 0.15F;	  // Min to move robot
	const float kMaxPwmOutput  = 0.70F;

	const float kRollTolerance	= 3.F;	 // in degrees
	const float kPitchTolerance = 3.F;	 // in degrees

	bool should_stop = true;
};

}	// namespace leka
