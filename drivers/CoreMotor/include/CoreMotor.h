// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "drivers/PwmOut.h"
#include "drivers/interfaces/InterfaceDigitalOut.h"

#include "CoreMotorBase.h"
#include "interface/drivers/PwmOut.h"

namespace leka {

class CoreMotor : public CoreMotorBase
{
  public:
	CoreMotor(mbed::interface::DigitalOut &dir_1, mbed::interface::DigitalOut &dir_2, interface::PwmOut &speed)
		: _dir_1 {dir_1}, _dir_2 {dir_2}, _speed {speed} {};

	void spin(rotation_t rotation, float speed) override;
	void stop() override;

  private:
	void setDirections(int dir_1, int dir_2);
	void setSpeed(float speed);

	mbed::interface::DigitalOut &_dir_1;
	mbed::interface::DigitalOut &_dir_2;
	interface::PwmOut &_speed;
};

}	// namespace leka
