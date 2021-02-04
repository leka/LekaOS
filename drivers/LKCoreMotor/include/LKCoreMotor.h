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
	// TODO: stub DigitalOut::read()
	struct Status {
		int dir_1;
		int dir_2;
		float speed;
	};

	LKCoreMotor(PinName direction_1, PinName direction_2, PinName speed)
		: _dir_1 {mbed::DigitalOut(direction_1)},
		  _dir_2 {mbed::DigitalOut(direction_2)},
		  _speed {mbed::PwmOut(speed)} {};

	void spin(rotation_t rotation, float speed) override;
	void stop(void) override;

	Status getStatus(void) const;

  private:
	Status _status {0, 0, 0};

	mbed::DigitalOut _dir_1;
	mbed::DigitalOut _dir_2;
	mbed::PwmOut _speed;

	void setDirections(int dir_1, int dir_2);
	void setSpeed(float speed);
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_MOTOR_H_
