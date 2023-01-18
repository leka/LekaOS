// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"

#include "CoreI2C.h"
#include "CoreLSM6DSOX.h"
#include "HelloWorld.h"
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

}	// namespace

auto main() -> int
{
	logger::init();

	HelloWorld hello;
	hello.start();

	imu::lsm6dsox.init();

	imu::lsm6dsox.setPowerMode(CoreLSM6DSOX::PowerMode::Off);
	imu::lsm6dsox.setPowerMode(CoreLSM6DSOX::PowerMode::Normal);

	auto callback = [](const interface::LSM6DSOX::SensorData &imu_data) {
		const auto &[xlx, xly, xlz] = imu_data.xl;
		const auto &[gx, gy, gz]	= imu_data.gy;
		log_info("Xl : x: %7.2f, y: %7.2f, z: %7.2f\n", xlx, xly, xlz);
		log_info("Gy : x: %7.2f, y: %7.2f, z: %7.2f\n", gx, gy, gz);
	};

	imu::lsm6dsox.registerOnGyDataReadyCallback(callback);

	while (true) {
		rtos::ThisThread::sleep_for(1s);
	}
}
