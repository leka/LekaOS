// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cinttypes>

#include "rtos/ThisThread.h"

#include "CoreI2C.h"
#include "CoreIMU.hpp"
#include "HelloWorld.h"
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

}	// namespace

auto main() -> int
{
	logger::init();

	HelloWorld hello;
	hello.start();

	imu::coreimu.init();

	imu::coreimu.setPowerMode(CoreIMU::PowerMode::Off);

	auto callback = [](const interface::IMU::SensorData data) {
		const auto &[xlx, xly, xlz] = data.xl;
		const auto &[gx, gy, gz]	= data.gy;
		const auto timestamp		= data.timestamp.time_since_epoch().count();

		log_debug("ts: %" PRId64 "ms, xl.x: %7.2f, xl.y: %7.2f, xl.z: %7.2f, gy.x: %7.2f, gy.y: %7.2f, gy.z: %7.2f",
				  timestamp, xlx, xly, xlz, gx, gy, gz);
	};

	imu::coreimu.registerOnDataReadyCallback(callback);

	while (true) {
		log_info("Setting normal power mode for 5s");
		rtos::ThisThread::sleep_for(1s);
		imu::coreimu.setPowerMode(CoreIMU::PowerMode::Normal);

		rtos::ThisThread::sleep_for(5s);

		imu::coreimu.setPowerMode(CoreIMU::PowerMode::Off);
		rtos::ThisThread::sleep_for(500ms);
		log_info("Turning off for 5s");

		rtos::ThisThread::sleep_for(5s);
	}
}
