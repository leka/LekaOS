// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_MOTOR_BASE_H_
#define _LEKA_OS_LIB_MOTOR_BASE_H_

// #include "mbed.h"

enum class Rotation
{
	clockwise,
	counterClockwise
};

using rotation_t = Rotation;

class MotorBase
{
  public:
	virtual void spin(rotation_t rotation, float speed) = 0;
	virtual void stop(void)								= 0;
};

#endif	 // _LEKA_OS_LIB_MOTOR_BASE_H_
