// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_MOTOR_H_
#define _LEKA_OS_LIB_MOTOR_H_

#include "drivers/PwmOut.h"
#include "drivers/interfaces/InterfaceDigitalOut.h"

#include "CoreMotorBase.h"

namespace leka {

class CoreMotor : public CoreMotorBase
{
  public:
	// TODO: stub DigitalOut::read()
	struct Status {
		int dir_1;
		int dir_2;
		float speed;
	};

	CoreMotor(mbed::interface::DigitalOut &dir_1, mbed::interface::DigitalOut &dir_2, PinName speed)
		: _dir_1 {dir_1}, _dir_2 {dir_2}, _speed {mbed::PwmOut(speed)} {};

	void spin(rotation_t rotation, float speed) override;
	void stop() override;

	[[nodiscard]] auto getStatus() const -> Status;

  private:
	Status _status {0, 0, 0};

	mbed::interface::DigitalOut &_dir_1;
	mbed::interface::DigitalOut &_dir_2;
	mbed::PwmOut _speed;

	void setDirections(int dir_1, int dir_2);
	void setSpeed(float speed);
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_MOTOR_H_
