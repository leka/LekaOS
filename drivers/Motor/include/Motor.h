// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_MOTOR_H_
#define _LEKA_OS_LIB_MOTOR_H_

#include "drivers/DigitalOut.h"
#include "drivers/PwmOut.h"

#include "MotorBase.h"

namespace leka {

class Motor : public MotorBase
{
  public:
	Motor(mbed::DigitalOut &direction_1, mbed::DigitalOut &direction_2, mbed::PwmOut &speed);

	void spin(rotation_t rotation, float speed) final;
	void stop(void) final;

  private:
	mbed::DigitalOut &_direction_1;
	mbed::DigitalOut &_direction_2;
	mbed::PwmOut &_speed;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_MOTOR_H_
