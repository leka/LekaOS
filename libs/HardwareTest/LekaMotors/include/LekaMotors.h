// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_MOTORS_H_
#define _LEKA_OS_LIB_MOTORS_H_

// #include <chrono>

// #include "PinNames.h"

#include "drivers/DigitalOut.h"
#include "drivers/PwmOut.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "CoreMotorBase.h"
#include "TwoMotorsBase.h"

namespace leka {

class Motors : TwoMotorsBase
{
  public:
	Motors(CoreMotorBase &motor_right, CoreMotorBase &motor_left);
	~Motors() {};

	void move(direction_t direction, uint8_t speed);
	void spin(rotation_t rotation, uint8_t speed);
	void stop(void);

  private:
	CoreMotorBase &_motor_right;
	CoreMotorBase &_motor_left;
};

}	// namespace leka

#endif
