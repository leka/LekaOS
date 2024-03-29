// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/libs/IMUKit.hpp"
namespace leka {

class RotationControl
{
  public:
	RotationControl() = default;

	void setTarget(EulerAngles starting_angles, float degrees);
	auto processRotationAngle(EulerAngles current_angles) -> float;

  private:
	void calculateTotalYawRotation(EulerAngles angle);
	[[nodiscard]] auto mapSpeed(float speed) const -> float;

	// ? Kp, Ki, Kd were found empirically by increasing Kp until the rotation angle exceeds the target angle
	// ? Then increase Kd to fix this excess angle
	// ? Repeat this protocol until there is no Kd high enough to compensate Kp
	// ? Then take the last set of Kp, Kd value with no excess angle
	// ? Finally choose a low Ki that smooth out the movement
	struct Parameters {
		static constexpr auto Kp = float {0.3F};
		static constexpr auto Ki = float {0.0001F};
		static constexpr auto Kd = float {0.4F};
	};

	static constexpr float kStaticBound = 5.F;
	static constexpr float kDeltaT		= 20.F;

	// ? When the motor is stopped, PWM values under kMinimalViableRobotPwm are too low to generate enough torque for
	// ? the motor to start spinning ? At the same time, kMinimalViableRobotPwm needs to be the lowest possible to avoid
	// ? overshooting when the target is reached
	static constexpr float kMinimalViableRobotPwm = 0.15F;
	static constexpr float kPwmMaxValue			  = 1.F;
	static constexpr float kEpsilon				  = 0.005F;

	static constexpr float kInputSpeedLimit = 90 * (Parameters::Kp + Parameters::Kd) / kDeltaT;

	float _angle_rotation_target = 0.F;
	float _angle_rotation_sum	 = 0.F;

	EulerAngles _euler_angles_previous {};

	float _error_position_total = 0.F;
	float _error_position_last	= 0.F;
	float _proportional			= 0.F;
	float _integral				= 0.F;
	float _derivative			= 0.F;
};

}	// namespace leka
