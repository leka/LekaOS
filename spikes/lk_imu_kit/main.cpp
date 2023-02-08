// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"

#include "CoreI2C.h"
#include "CoreLSM6DSOX.hpp"
#include "HelloWorld.h"
#include "IMUKit.hpp"
#include "LogKit.h"

using namespace std::chrono;
using namespace leka;

namespace {

namespace imu {

	namespace internal {

		auto drdy_irq = CoreInterruptIn {PinName::SENSOR_IMU_IRQ};
		auto i2c	  = CoreI2C(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);

	}	// namespace internal

	CoreLSM6DSOX lsm6dsox(internal::i2c, internal::drdy_irq);

}	// namespace imu

IMUKit imukit(imu::lsm6dsox);

}	// namespace

auto main() -> int
{
	rtos::ThisThread::sleep_for(140ms);
	logger::init();

	HelloWorld hello;
	hello.start();

	imu::lsm6dsox.init();

	imukit.stop();
	imukit.init();
	imukit.start();

	while (true) {
		const auto [pitch, roll, yaw] = imukit.getEulerAngles();
		log_info("Pitch : %7.2f, Roll : %7.2f Yaw : %7.2f", pitch, roll, yaw);
		rtos::ThisThread::sleep_for(140ms);
	}
}
