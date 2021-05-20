// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_MOTOR_BASE_H_
#define _LEKA_OS_LIB_MOTOR_BASE_H_

namespace leka {

enum class Rotation
{
	clockwise		 = 0,
	counterClockwise = 1,

	right = clockwise,
	left  = counterClockwise
};

using rotation_t = Rotation;

class CoreMotorBase
{
  public:
	virtual void spin(rotation_t rotation, float speed) = 0;
	virtual void stop(void)								= 0;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_MOTOR_BASE_H_
