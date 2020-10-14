// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_MOTOR_H_
#define _LEKA_OS_LIB_MOTOR_H_

#include "drivers/DigitalOut.h"
#include "drivers/PwmOut.h"

#include "MotorBase.h"

using namespace mbed;

class Motor : public MotorBase
{
  public:
	Motor(DigitalOut &direction_1, DigitalOut &direction_2, PwmOut &speed);

	void spin(rotation_t rotation, float speed);
	void stop(void);

  private:
	DigitalOut &_direction_1;
	DigitalOut &_direction_2;
	PwmOut &_speed;
};

#endif	 // _LEKA_OS_LIB_MOTOR_H_
