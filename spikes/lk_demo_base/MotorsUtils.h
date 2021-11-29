// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_MOTORS_UTILS_H_
#define _LEKA_OS_MOTORS_UTILS_H_

#include "CoreMotor.h"
#include "CorePwm.h"

namespace leka {

class MotorsUtils
{
  public:
	enum class SpinDirection
	{
		Left,
		Right
	};

	explicit MotorsUtils(CoreMotor &motor_left, CoreMotor &motor_right, CorePwm &motor_left_speed,
						 CorePwm &motor_right_speed)
		: _motor_left(motor_left),
		  _motor_right(motor_right),
		  _motor_left_speed(motor_left_speed),
		  _motor_right_speed(motor_right_speed) {};

	void setSpeed(float left_speed, float right_speed);

	void spin(SpinDirection direction);
	void stop();

  private:
	CoreMotor &_motor_left;
	CoreMotor &_motor_right;

	CorePwm &_motor_left_speed;
	CorePwm &_motor_right_speed;

	float _left_speed {1.F};
	float _right_speed {1.F};
};

}	// namespace leka

#endif	 // _LEKA_OS_MOTORS_UTILS_H_
