// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_TWO_MOTORS_BASE_H_
#define _LEKA_OS_LIB_TWO_MOTORS_BASE_H_

enum class Direction
{
	forward,
	backward
};

using direction_t = Direction;

class TwoMotorsBase
{
  public:
	virtual void move(direction_t direction, uint8_t speed) = 0;
	virtual void spin(rotation_t rotation, uint8_t speed)	= 0;
	virtual void stop(void)									= 0;
};

#endif	 // _LEKA_OS_LIB_TWO_MOTORS_BASE_H_
