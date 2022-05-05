// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// #include <chrono>

// #include "PinNames.h"

#include "drivers/DigitalOut.h"
#include "drivers/PwmOut.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "TwoMotorsBase.h"
#include "interface/drivers/Motor.h"

namespace leka {

class Motors : TwoMotorsBase
{
  public:
	Motors(interface::Motor &motor_right, interface::Motor &motor_left);
	~Motors() {};

	void move(direction_t direction, uint8_t speed);
	void spin(rotation_t rotation, uint8_t speed);
	void stop(void);

  private:
	interface::Motor &_motor_right;
	interface::Motor &_motor_left;
};

}	// namespace leka
