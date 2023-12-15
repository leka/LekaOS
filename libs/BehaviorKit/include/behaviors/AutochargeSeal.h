// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/Behavior.h"
#include "interface/drivers/Motor.h"
#include "interface/libs/IMUKit.hpp"

namespace leka::behavior {

class AutochargeSeal : public interface::Behavior
{
  public:
	explicit AutochargeSeal(interface::Motor &motor_left, interface::Motor &motor_right, interface::IMUKit &imu_kit)
		: _motor_left(motor_left), _motor_right(motor_right), _imukit(imu_kit)
	{
		// nothing to do
	}

	auto id() -> BehaviorID final;

	void run() final;
	void stop() final;

  private:
	[[nodiscard]] auto convertToPwmFrom(float angle) const -> float;

	void stopMotors();
	void spinRight(float left_speed, float right_speed);
	void spinLeft(float left_speed, float right_speed);
	void moveForward(float speed);
	void moveBackward(float speed);

	void spinToFixRoll(bool is_right_tilted, bool should_move_forward, float speed_offset);
	void moveToFixPitch(bool should_move_forward, float speed);

	const float kMinAngleInput = 0.F;
	const float kMaxAngleInput = 30.F;
	const float kMinPwmOutput  = 0.15F;	  // Min to move robot
	const float kMaxPwmOutput  = 0.70F;

	const float kRollTolerance	= 3.F;	 // in degrees
	const float kPitchTolerance = 3.F;	 // in degrees

	interface::Motor &_motor_left;
	interface::Motor &_motor_right;
	interface::IMUKit &_imukit;

	BehaviorID _id {behavior_id::autocharge_seal};

	bool must_stop {true};
};

}	// namespace leka::behavior
