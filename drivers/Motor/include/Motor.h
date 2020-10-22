// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_MOTOR_H_
#define _LEKA_OS_LIB_MOTOR_H_

#include "drivers/DigitalOut.h"
#include "drivers/PwmOut.h"

#include "MotorBase.h"

class Motor : public MotorBase
{
  public:
	Motor(mbed::DigitalOut &direction_1, mbed::DigitalOut &direction_2, mbed::PwmOut &speed);

	void spin(rotation_t rotation, float speed);
	void stop(void);

  private:
	mbed::DigitalOut &_direction_1;
	mbed::DigitalOut &_direction_2;
	mbed::PwmOut &_speed;
};

#endif	 // _LEKA_OS_LIB_MOTOR_H_
