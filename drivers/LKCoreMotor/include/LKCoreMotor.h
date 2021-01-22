// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_MOTOR_H_
#define _LEKA_OS_LIB_MOTOR_H_

#include "drivers/DigitalOut.h"
#include "drivers/PwmOut.h"

#include "LKCoreMotorBase.h"

namespace leka {

class LKCoreMotor : public LKCoreMotorBase
{
  public:
	LKCoreMotor(PinName direction_1, PinName direction_2, PinName peed);

	void spin(rotation_t rotation, float speed) override;
	void stop(void) override;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_MOTOR_H_
