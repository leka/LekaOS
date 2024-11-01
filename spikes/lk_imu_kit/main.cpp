// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"

#include "CoreI2C.h"
#include "CoreIMU.hpp"
#include "HelloWorld.h"
#include "IMUKit.hpp"
#include "LogKit.h"

using namespace std::chrono;
using namespace leka;

namespace {

namespace imu {

	namespace internal {

		auto irq = CoreInterruptIn {PinName::SENSOR_IMU_IRQ};
		auto i2c = CoreI2C(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);

	}	// namespace internal

	CoreIMU coreimu(internal::i2c, internal::irq);

}	// namespace imu

IMUKit imukit(imu::coreimu);

}	// namespace

auto main() -> int
{
	rtos::ThisThread::sleep_for(140ms);
	logger::init();

	HelloWorld hello;
	hello.start();

	imu::coreimu.init();

	imukit.stop();
	imukit.init();
	imukit.start();

	while (true) {
		const auto [pitch, roll, yaw] = imukit.getEulerAngles();
		log_info("Pitch : %i, Roll : %i Yaw : %i", static_cast<int>(pitch), static_cast<int>(roll),
				 static_cast<int>(yaw));
		rtos::ThisThread::sleep_for(140ms);
	}
}
