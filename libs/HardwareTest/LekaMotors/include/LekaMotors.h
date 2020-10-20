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

#include "MotorBase.h"
#include "TwoMotorsBase.h"

using namespace mbed;

class Motors : TwoMotorsBase
{
  public:
	Motors(MotorBase &motor_right, MotorBase &motor_left);
	~Motors() {};

	void translate(translation_t translation, float speed);
	void spin(turn_t turn, float speed);
	void stop(void);
	void brake(void);

  private:
	MotorBase &_motor_right;
	MotorBase &_motor_left;
};

#endif
